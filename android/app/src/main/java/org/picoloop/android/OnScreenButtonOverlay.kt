package org.picoloop.android

import android.content.Context
import android.content.SharedPreferences
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.RectF
import android.view.KeyEvent
import android.view.MotionEvent
import android.view.View
import kotlin.math.max
import kotlin.math.min

/**
 * Touch d-pad + A/B/L/R/Start/Select overlay drawn on top of the SDL surface.
 *
 * picoloop reads input as SDLK_* keycodes (see the __ANDROID__ block in
 * Master.h, which reuses the same keymap as the desktop/PC_DESKTOP build:
 * BUTTON_A=SDLK_LCTRL, BUTTON_B=SDLK_LALT, arrows=SDLK_UP/DOWN/LEFT/RIGHT,
 * BUTTON_START=SDLK_RETURN, BUTTON_SELECT=SDLK_ESCAPE, BUTTON_L=SDLK_TAB,
 * BUTTON_R=SDLK_BACKSPACE). So instead of teaching the native side a new set
 * of Android-specific button codes, each on-screen button below is just
 * given the Android KEYCODE_* that SDL2's own Android backend already
 * translates into that same SDLK_* - no native-side changes needed here.
 * Only the drawn label changes with the PC/PSP display mode (see
 * applyLabelMode()); the picoloop manuals name the same controls
 * differently (Ctrl/Alt/Esc/Enter/Tab/Backspace vs A/B/Select/Start/L/R).
 *
 * Layout (button positions/size, and separately the game screen's own
 * position) is user-editable from the options screen (SettingsActivity ->
 * edit mode here) and persisted in the "picoloop_prefs" SharedPreferences.
 * Portrait and landscape are independent: a phone held upright and the same
 * phone held sideways have very different usable areas, so positions saved
 * in one don't carry over to the other.
 */
class OnScreenButtonOverlay(context: Context) : View(context) {
    data class ButtonDef(
        val id: String,
        var label: String,
        val keyCode: Int,
        var bounds: RectF = RectF(),
        var isPressed: Boolean = false,
        var offsetXFrac: Float = 0f,
        var offsetYFrac: Float = 0f
    )

    private val up = ButtonDef("up", "↑", KeyEvent.KEYCODE_DPAD_UP)
    private val down = ButtonDef("down", "↓", KeyEvent.KEYCODE_DPAD_DOWN)
    private val left = ButtonDef("left", "←", KeyEvent.KEYCODE_DPAD_LEFT)
    private val right = ButtonDef("right", "→", KeyEvent.KEYCODE_DPAD_RIGHT)
    private val a = ButtonDef("a", "A", KeyEvent.KEYCODE_CTRL_LEFT)
    private val b = ButtonDef("b", "B", KeyEvent.KEYCODE_ALT_LEFT)
    private val start = ButtonDef("start", "Start", KeyEvent.KEYCODE_ENTER)
    private val select = ButtonDef("select", "Select", KeyEvent.KEYCODE_ESCAPE)
    private val l = ButtonDef("l", "L", KeyEvent.KEYCODE_TAB)
    private val r = ButtonDef("r", "R", KeyEvent.KEYCODE_DEL)

    private val buttons = listOf(up, down, left, right, a, b, start, select, l, r)

    private val prefs: SharedPreferences =
        context.getSharedPreferences("picoloop_prefs", Context.MODE_PRIVATE)

    var editMode: Boolean = false
        set(value) {
            field = value
            touchMap.clear()
            draggingId = null
            invalidate()
        }

    // Moves the whole picoloop screen (not the buttons) - see
    // SDL_GUI.cpp's __ANDROID__ refresh(), which reads the offset this
    // pushes over JNI via onScreenOffsetChanged.
    var screenEditMode: Boolean = false
        set(value) {
            field = value
            invalidate()
        }

    var scale: Float = 1.0f
        private set

    var screenOffsetXFrac: Float = 0f
        private set
    var screenOffsetYFrac: Float = 0f
        private set

    // Fired whenever the screen offset changes (orientation switch, live
    // drag, or reset) so the activity can forward it to native code.
    var onScreenOffsetChanged: ((Float, Float) -> Unit)? = null

    private var lastWidth = 0
    private var lastHeight = 0
    private var draggingId: String? = null
    private var screenDragLastX = 0f
    private var screenDragLastY = 0f
    // Portrait and landscape get independently edited/saved layouts (button
    // positions make sense relative to a wide-vs-tall canvas very
    // differently), keyed by this suffix. Reloaded in onSizeChanged()
    // whenever a resize crosses the landscape/portrait boundary (rotation,
    // or the orientation preference forcing one shortly after launch).
    private var currentOrientationSuffix: String? = null

    private val paint = Paint().apply {
        color = 0x80FFFFFF.toInt()
        style = Paint.Style.FILL
        isAntiAlias = true
    }
    private val paintPressed = Paint().apply {
        color = 0xC0FFFFFF.toInt()
        style = Paint.Style.FILL
        isAntiAlias = true
    }
    private val paintStroke = Paint().apply {
        color = 0x80000000.toInt()
        style = Paint.Style.STROKE
        strokeWidth = 2f
        isAntiAlias = true
    }
    private val paintEditStroke = Paint().apply {
        color = 0xFFFF3030.toInt()
        style = Paint.Style.STROKE
        strokeWidth = 5f
        isAntiAlias = true
    }
    private val textPaint = Paint().apply {
        color = 0xFF000000.toInt()
        textAlign = Paint.Align.CENTER
        isAntiAlias = true
    }

    private val touchMap = mutableMapOf<Int, ButtonDef>()

    init {
        applyLabelMode()
    }

    fun applyLabelMode() {
        val pc = prefs.getString(PREF_LABEL_MODE, "psp") == "pc"
        a.label = if (pc) "Ctrl" else "A"
        b.label = if (pc) "Alt" else "B"
        select.label = if (pc) "Esc" else "Select"
        start.label = if (pc) "Enter" else "Start"
        l.label = if (pc) "Tab" else "L"
        r.label = if (pc) "⌫" else "R"
        invalidate()
    }

    private fun orientationSuffix(w: Int, h: Int) = if (w >= h) "land" else "port"

    private fun loadLayoutForOrientation(suffix: String) {
        scale = prefs.getFloat(PREF_SCALE_PREFIX + suffix, 1.0f)
        for (button in buttons) {
            button.offsetXFrac = prefs.getFloat(PREF_OFFSET_X_PREFIX + suffix + "_" + button.id, 0f)
            button.offsetYFrac = prefs.getFloat(PREF_OFFSET_Y_PREFIX + suffix + "_" + button.id, 0f)
        }
        screenOffsetXFrac = prefs.getFloat(PREF_SCREEN_OFF_X_PREFIX + suffix, 0f)
        screenOffsetYFrac = prefs.getFloat(PREF_SCREEN_OFF_Y_PREFIX + suffix, 0f)
        onScreenOffsetChanged?.invoke(screenOffsetXFrac, screenOffsetYFrac)
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        lastWidth = w
        lastHeight = h
        val suffix = orientationSuffix(w, h)
        if (suffix != currentOrientationSuffix) {
            currentOrientationSuffix = suffix
            loadLayoutForOrientation(suffix)
        }
        layoutButtons(w, h)
    }

    private fun layoutButtons(width: Int, height: Int) {
        // Both dimensions, not just height: in portrait, height is the
        // *long* side, and sizing off it alone made every button huge
        // enough to overlap its neighbours.
        val base = min(width, height).toFloat()
        val btn = (base * 0.11f) * scale
        val margin = (base * 0.04f)
        textPaint.textSize = btn * 0.5f

        // D-pad, bottom-left, cross layout.
        val dpadCx = margin + btn * 1.5f
        val dpadCy = height - margin - btn * 1.5f
        left.bounds = RectF(dpadCx - btn * 1.5f, dpadCy - btn / 2, dpadCx - btn / 2, dpadCy + btn / 2)
        right.bounds = RectF(dpadCx + btn / 2, dpadCy - btn / 2, dpadCx + btn * 1.5f, dpadCy + btn / 2)
        up.bounds = RectF(dpadCx - btn / 2, dpadCy - btn * 1.5f, dpadCx + btn / 2, dpadCy - btn / 2)
        down.bounds = RectF(dpadCx - btn / 2, dpadCy + btn / 2, dpadCx + btn / 2, dpadCy + btn * 1.5f)

        // A/B, bottom-right, diagonal layout (B lower-left of A, like a handheld).
        val abCx = width - margin - btn * 1.5f
        val abCy = height - margin - btn * 1.5f
        b.bounds = RectF(abCx - btn / 2, abCy + btn * 0.2f, abCx + btn / 2, abCy + btn * 1.2f)
        a.bounds = RectF(abCx + btn * 0.6f, abCy - btn * 0.8f, abCx + btn * 1.6f, abCy + btn * 0.2f)

        // Start/Select, bottom-center.
        val scY = height - margin - btn * 0.6f
        select.bounds = RectF(width / 2f - btn * 1.6f, scY - btn * 0.35f, width / 2f - btn * 0.2f, scY + btn * 0.35f)
        start.bounds = RectF(width / 2f + btn * 0.2f, scY - btn * 0.35f, width / 2f + btn * 1.6f, scY + btn * 0.35f)

        // L/R shoulders, top corners.
        l.bounds = RectF(margin, margin, margin + btn * 1.6f, margin + btn * 0.8f)
        r.bounds = RectF(width - margin - btn * 1.6f, margin, width - margin, margin + btn * 0.8f)

        // Apply each button's saved (or currently-being-dragged) offset, in
        // fractions of the screen so it stays reasonable across rotation /
        // different devices.
        for (button in buttons) {
            val dx = button.offsetXFrac * width
            val dy = button.offsetYFrac * height
            button.bounds.offset(dx, dy)
        }
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        // Hide the buttons while dragging the game screen itself so the
        // whole picoloop canvas is visible and unobstructed underneath.
        if (screenEditMode) return
        buttons.forEach { button ->
            val p = if (button.isPressed) paintPressed else paint
            val radius = button.bounds.height() / 4
            canvas.drawRoundRect(button.bounds, radius, radius, p)
            canvas.drawRoundRect(button.bounds, radius, radius, if (editMode) paintEditStroke else paintStroke)
            canvas.drawText(
                button.label,
                button.bounds.centerX(),
                button.bounds.centerY() + textPaint.textSize / 3,
                textPaint
            )
        }
    }

    private fun dispatch(action: Int, button: ButtonDef) {
        dispatchKeyEvent(KeyEvent(action, button.keyCode))
    }

    fun setScale(newScale: Float) {
        scale = newScale.coerceIn(0.5f, 2.5f)
        if (lastWidth > 0) layoutButtons(lastWidth, lastHeight)
        invalidate()
    }

    fun saveLayout() {
        val suffix = currentOrientationSuffix ?: orientationSuffix(lastWidth, lastHeight)
        val editor = prefs.edit()
        editor.putFloat(PREF_SCALE_PREFIX + suffix, scale)
        for (button in buttons) {
            editor.putFloat(PREF_OFFSET_X_PREFIX + suffix + "_" + button.id, button.offsetXFrac)
            editor.putFloat(PREF_OFFSET_Y_PREFIX + suffix + "_" + button.id, button.offsetYFrac)
        }
        editor.apply()
    }

    fun resetLayout() {
        scale = 1.0f
        for (button in buttons) {
            button.offsetXFrac = 0f
            button.offsetYFrac = 0f
        }
        if (lastWidth > 0) layoutButtons(lastWidth, lastHeight)
        invalidate()
    }

    fun saveScreenOffset() {
        val suffix = currentOrientationSuffix ?: orientationSuffix(lastWidth, lastHeight)
        prefs.edit()
            .putFloat(PREF_SCREEN_OFF_X_PREFIX + suffix, screenOffsetXFrac)
            .putFloat(PREF_SCREEN_OFF_Y_PREFIX + suffix, screenOffsetYFrac)
            .apply()
    }

    fun resetScreenOffset() {
        screenOffsetXFrac = 0f
        screenOffsetYFrac = 0f
        onScreenOffsetChanged?.invoke(screenOffsetXFrac, screenOffsetYFrac)
        invalidate()
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (screenEditMode) return onTouchEventScreenMove(event)
        if (editMode) return onTouchEventEdit(event)

        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN, MotionEvent.ACTION_POINTER_DOWN -> {
                val idx = event.actionIndex
                val id = event.getPointerId(idx)
                val button = buttons.find { it.bounds.contains(event.getX(idx), event.getY(idx)) }
                if (button != null) {
                    button.isPressed = true
                    touchMap[id] = button
                    dispatch(KeyEvent.ACTION_DOWN, button)
                    invalidate()
                    return true
                }
                return false
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_POINTER_UP -> {
                val idx = event.actionIndex
                val id = event.getPointerId(idx)
                val button = touchMap.remove(id)
                if (button != null) {
                    button.isPressed = false
                    dispatch(KeyEvent.ACTION_UP, button)
                    invalidate()
                    return true
                }
                return false
            }
            MotionEvent.ACTION_MOVE -> {
                if (touchMap.isEmpty()) return false
                for (i in 0 until event.pointerCount) {
                    val id = event.getPointerId(i)
                    val old = touchMap[id] ?: continue
                    val x = event.getX(i)
                    val y = event.getY(i)
                    // Hysteresis: don't drop "old" just because the touch
                    // left its exact bounds. Finger jitter right on a
                    // button's edge (small adjacent d-pad buttons are the
                    // worst case) was flipping contains() in and out on
                    // every sample, causing an UP+DOWN pair per flicker -
                    // each one a fresh press edge, so a single tap could
                    // register as 2 (or more) engine steps. Only release
                    // once the touch is clearly outside a padded rect.
                    if (!old.bounds.contains(x, y)) {
                        val marginX = old.bounds.width() * 0.35f
                        val marginY = old.bounds.height() * 0.35f
                        val padded = RectF(
                            old.bounds.left - marginX, old.bounds.top - marginY,
                            old.bounds.right + marginX, old.bounds.bottom + marginY
                        )
                        if (padded.contains(x, y)) continue
                        val now = buttons.find { it.bounds.contains(x, y) }
                        old.isPressed = false
                        dispatch(KeyEvent.ACTION_UP, old)
                        if (now != null) {
                            now.isPressed = true
                            touchMap[id] = now
                            dispatch(KeyEvent.ACTION_DOWN, now)
                        } else {
                            touchMap.remove(id)
                        }
                        invalidate()
                    }
                }
                return true
            }
            MotionEvent.ACTION_CANCEL -> {
                touchMap.values.forEach { it.isPressed = false; dispatch(KeyEvent.ACTION_UP, it) }
                touchMap.clear()
                invalidate()
                return true
            }
        }
        return false
    }

    // In edit mode, a touch drags whichever button it started on; no key
    // events are ever dispatched so dragging never triggers gameplay input.
    private fun onTouchEventEdit(event: MotionEvent): Boolean {
        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                val x = event.x
                val y = event.y
                val button = buttons.find { it.bounds.contains(x, y) }
                draggingId = button?.id
                return button != null
            }
            MotionEvent.ACTION_MOVE -> {
                val id = draggingId ?: return false
                val button = buttons.find { it.id == id } ?: return false
                val width = max(lastWidth, 1)
                val height = max(lastHeight, 1)
                val cx = button.bounds.centerX()
                val cy = button.bounds.centerY()
                val dx = event.x - cx
                val dy = event.y - cy
                button.offsetXFrac = clampOffset(button.offsetXFrac + dx / width)
                button.offsetYFrac = clampOffset(button.offsetYFrac + dy / height)
                layoutButtons(width, height)
                invalidate()
                return true
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                draggingId = null
                return true
            }
        }
        return false
    }

    // A touch anywhere drags the whole game screen (native side clamps it
    // to however much letterbox slack actually exists on each axis - see
    // SDL_GUI.cpp's __ANDROID__ refresh()).
    private fun onTouchEventScreenMove(event: MotionEvent): Boolean {
        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                screenDragLastX = event.x
                screenDragLastY = event.y
                return true
            }
            MotionEvent.ACTION_MOVE -> {
                val width = max(lastWidth, 1)
                val height = max(lastHeight, 1)
                val dx = event.x - screenDragLastX
                val dy = event.y - screenDragLastY
                screenDragLastX = event.x
                screenDragLastY = event.y
                screenOffsetXFrac = clampScreenOffset(screenOffsetXFrac + dx / width)
                screenOffsetYFrac = clampScreenOffset(screenOffsetYFrac + dy / height)
                onScreenOffsetChanged?.invoke(screenOffsetXFrac, screenOffsetYFrac)
                return true
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> return true
        }
        return false
    }

    private fun clampOffset(v: Float) = min(0.4f, max(-0.4f, v))
    private fun clampScreenOffset(v: Float) = min(0.5f, max(-0.5f, v))

    override fun dispatchKeyEvent(event: KeyEvent): Boolean {
        return (context as? android.app.Activity)?.dispatchKeyEvent(event) ?: super.dispatchKeyEvent(event)
    }

    companion object {
        // "_land"/"_port" suffixed at point of use, see orientationSuffix().
        private const val PREF_SCALE_PREFIX = "btn_scale_"
        private const val PREF_OFFSET_X_PREFIX = "btn_off_x_"
        private const val PREF_OFFSET_Y_PREFIX = "btn_off_y_"
        private const val PREF_SCREEN_OFF_X_PREFIX = "screen_off_x_"
        private const val PREF_SCREEN_OFF_Y_PREFIX = "screen_off_y_"
        const val PREF_LABEL_MODE = "button_label_mode"
    }
}
