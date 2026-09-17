package org.picoloop.android

import android.content.Intent
import android.content.SharedPreferences
import android.content.pm.ActivityInfo
import android.graphics.Color
import android.os.Bundle
import android.view.Gravity
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import android.widget.ImageButton
import android.widget.LinearLayout
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import org.libsdl.app.SDLActivity

/**
 * picoloop is built with -D__SDL_AUDIO__ (see Makefile.PatternPlayer_android_SDL2),
 * so SDL2's own audio backend (AAudio/OpenSL ES on Android) is used - no extra
 * audio glue needed here, unlike apps built against RtAudio.
 */
class PicoloopActivity : SDLActivity() {
    private var overlaySetupDone = false
    private var buttonOverlay: OnScreenButtonOverlay? = null
    private var editBar: LinearLayout? = null
    private lateinit var prefs: SharedPreferences

    // Implemented in SDL_GUI.cpp's __ANDROID__ refresh(); pushes the game
    // screen's own position (as a fraction of the window, clamped natively
    // to however much letterbox slack actually exists on each axis).
    private external fun nativeSetScreenOffset(x: Float, y: Float)

    override fun onCreate(savedInstanceState: Bundle?) {
        // Must happen before super.onCreate(): that's where SDLActivity
        // creates the native window/surface, and picoloop's own drawing is
        // a fixed-size offscreen buffer scaled onto whatever surface size
        // SDL captures at that moment (see SDL_GUI.cpp's __ANDROID__
        // initVideo()/refresh()). Setting the orientation afterwards (e.g.
        // from onResume()) rotates the *visible* screen but SDL's captured
        // windowSurface stays stale at the old (wrong) size, so content
        // gets scaled/positioned for a canvas that no longer matches what's
        // on screen - the app looks like it's rendering nothing.
        prefs = getSharedPreferences("picoloop_prefs", MODE_PRIVATE)
        applyOrientationPreference()

        super.onCreate(savedInstanceState)
        hideSystemBars()
        // native MidiInSystem/MidiOutSystem (see picoloop/MidiInSystem.cpp,
        // MidiOutSystem.cpp __ANDROID__ paths) call back into MidiBridge via
        // JNI, so it needs a MidiManager before init_midi() runs.
        MidiBridge.init(applicationContext)
    }

    // The status bar otherwise overlaps the L/R buttons and the gear icon
    // in landscape. Android also reveals the system bars again after any
    // swipe-from-edge or loss of focus (e.g. returning from the options
    // screen), so this needs reapplying on every focus regain, not just once.
    private fun hideSystemBars() {
        WindowCompat.setDecorFitsSystemWindows(window, false)
        WindowInsetsControllerCompat(window, window.decorView).apply {
            hide(WindowInsetsCompat.Type.systemBars())
            systemBarsBehavior = WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        }
    }

    override fun getLibraries(): Array<String> {
        return arrayOf(
            "SDL2",
            "SDL2_ttf",
            "main"
        )
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemBars()
        }
        if (hasFocus && !overlaySetupDone) {
            prefs = getSharedPreferences("picoloop_prefs", MODE_PRIVATE)
            setupButtonOverlay()
            setupGearButton()
            overlaySetupDone = true
        }
    }

    override fun onResume() {
        super.onResume()
        if (overlaySetupDone) {
            applyOrientationPreference()
            buttonOverlay?.applyLabelMode()
            if (prefs.getBoolean(PREF_PENDING_EDIT_MODE, false)) {
                prefs.edit().putBoolean(PREF_PENDING_EDIT_MODE, false).apply()
                enterButtonEditMode()
            }
            if (prefs.getBoolean(PREF_PENDING_SCREEN_EDIT_MODE, false)) {
                prefs.edit().putBoolean(PREF_PENDING_SCREEN_EDIT_MODE, false).apply()
                enterScreenEditMode()
            }
        }
    }

    private fun applyOrientationPreference() {
        requestedOrientation = when (prefs.getString(PREF_ORIENTATION, "landscape")) {
            "portrait" -> ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
            "landscape" -> ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
            else -> ActivityInfo.SCREEN_ORIENTATION_USER
        }
    }

    private fun setupButtonOverlay() {
        val contentView = window.decorView.findViewById<ViewGroup>(android.R.id.content)
        val overlay = OnScreenButtonOverlay(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
            )
            onScreenOffsetChanged = { x, y -> nativeSetScreenOffset(x, y) }
        }
        contentView.addView(overlay)
        buttonOverlay = overlay
        applyOrientationPreference()
    }

    private fun setupGearButton() {
        val contentView = window.decorView.findViewById<ViewGroup>(android.R.id.content)
        val density = resources.displayMetrics.density
        val sizePx = (36 * density).toInt()
        val marginPx = (6 * density).toInt()

        val gear = ImageButton(this).apply {
            layoutParams = FrameLayout.LayoutParams(sizePx, sizePx, Gravity.TOP or Gravity.END).apply {
                topMargin = marginPx
                rightMargin = marginPx
            }
            setImageResource(android.R.drawable.ic_menu_manage)
            setBackgroundColor(Color.argb(120, 0, 0, 0))
            alpha = 0.6f
            elevation = 100f
            setOnClickListener { startActivity(Intent(this@PicoloopActivity, SettingsActivity::class.java)) }
        }
        contentView.addView(gear)
    }

    private fun enterButtonEditMode() {
        buttonOverlay?.editMode = true
        showEditBar(
            "-" to { buttonOverlay?.let { it.setScale(it.scale - 0.1f) } },
            "+" to { buttonOverlay?.let { it.setScale(it.scale + 0.1f) } },
            "Reinitialiser" to { buttonOverlay?.resetLayout() },
            "Termine" to { exitButtonEditMode() }
        )
    }

    private fun exitButtonEditMode() {
        buttonOverlay?.saveLayout()
        buttonOverlay?.editMode = false
        hideEditBar()
    }

    private fun enterScreenEditMode() {
        buttonOverlay?.screenEditMode = true
        showEditBar(
            "Reinitialiser" to { buttonOverlay?.resetScreenOffset() },
            "Termine" to { exitScreenEditMode() }
        )
    }

    private fun exitScreenEditMode() {
        buttonOverlay?.saveScreenOffset()
        buttonOverlay?.screenEditMode = false
        hideEditBar()
    }

    private fun showEditBar(vararg actions: Pair<String, () -> Unit>) {
        hideEditBar()
        val contentView = window.decorView.findViewById<ViewGroup>(android.R.id.content)
        val density = resources.displayMetrics.density

        val bar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            setBackgroundColor(Color.argb(200, 20, 20, 20))
            setPadding((8 * density).toInt(), (4 * density).toInt(), (8 * density).toInt(), (4 * density).toInt())
            layoutParams = FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                Gravity.TOP or Gravity.CENTER_HORIZONTAL
            ).apply { topMargin = (6 * density).toInt() }
            elevation = 100f
        }

        for ((label, action) in actions) {
            bar.addView(Button(this).apply {
                text = label
                setOnClickListener { action() }
            })
        }

        contentView.addView(bar)
        editBar = bar
    }

    private fun hideEditBar() {
        editBar?.let {
            (it.parent as? ViewGroup)?.removeView(it)
        }
        editBar = null
    }

    companion object {
        const val PREF_ORIENTATION = "orientation_mode"
        const val PREF_PENDING_EDIT_MODE = "pending_edit_mode"
        const val PREF_PENDING_SCREEN_EDIT_MODE = "pending_screen_edit_mode"
    }
}
