package org.picoloop.android

import android.app.Activity
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.graphics.Typeface
import android.os.Bundle
import android.text.InputType
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.RadioGroup
import android.widget.ScrollView
import android.widget.TextView
import android.widget.Toast
import com.google.android.material.button.MaterialButton
import com.google.android.material.card.MaterialCardView
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.radiobutton.MaterialRadioButton
import java.io.File
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import java.util.zip.ZipEntry
import java.util.zip.ZipInputStream
import java.util.zip.ZipOutputStream

/**
 * "picoloop options" screen. For now: on-screen button layout (position +
 * size, edited live over the game screen), the game screen's own position,
 * orientation, the PC/PSP button label style and its matching manual,
 * interface language, storage folder/theme/key-repeat/font (read from and
 * written to picoloop.ini in the active storage folder), bank backups and
 * a clean-quit button. More entries can be appended to `content` later.
 *
 * Material 3 (see Theme.Picoloop.Settings in themes.xml) - this screen
 * only, PicoloopActivity/MainActivity are untouched plain system themes.
 */
class SettingsActivity : Activity() {
    private lateinit var prefs: SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Strings.init(this)
        prefs = getSharedPreferences("picoloop_prefs", MODE_PRIVATE)

        val scroll = ScrollView(this).apply {
            setBackgroundColor(themeColor(com.google.android.material.R.attr.colorSurface))
        }
        val content = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(dp(16), dp(20), dp(16), dp(32))
        }
        scroll.addView(content)

        content.addView(pageTitle(Strings.t("options_title")))

        content.addView(card {
            addView(rowTitle(Strings.t("language_title")))
            addView(rowSubtitle(Strings.t("language_subtitle")))
            val langGroup = RadioGroup(this@SettingsActivity).apply { orientation = RadioGroup.VERTICAL }
            val optionEn = radio("English", 10)
            val optionFr = radio("Francais", 11)
            langGroup.addView(optionEn)
            langGroup.addView(optionFr)
            if (Strings.language == "fr") optionFr.isChecked = true else optionEn.isChecked = true
            langGroup.setOnCheckedChangeListener { _, checkedId ->
                Strings.language = if (checkedId == optionFr.id) "fr" else "en"
                recreate()
            }
            addView(langGroup)
        })

        content.addView(card {
            addView(rowTitle(Strings.t("layout_title")))
            addView(rowSubtitle(Strings.t("layout_subtitle")))
            addView(outlinedButton(Strings.t("layout_edit")) {
                prefs.edit().putBoolean(PicoloopActivity.PREF_PENDING_EDIT_MODE, true).apply()
                finish()
            })
        })

        content.addView(card {
            addView(rowTitle(Strings.t("screen_pos_title")))
            addView(rowSubtitle(Strings.t("screen_pos_subtitle")))
            addView(outlinedButton(Strings.t("screen_pos_edit")) {
                prefs.edit().putBoolean(PicoloopActivity.PREF_PENDING_SCREEN_EDIT_MODE, true).apply()
                finish()
            })
        })

        content.addView(card {
            addView(rowTitle(Strings.t("orientation_title")))
            val currentOrientation = prefs.getString(PicoloopActivity.PREF_ORIENTATION, "landscape")
            val group = RadioGroup(this@SettingsActivity).apply { orientation = RadioGroup.VERTICAL }
            val optionPaysage = radio(Strings.t("orientation_landscape"), 1)
            val optionPortrait = radio(Strings.t("orientation_portrait"), 2)
            val optionAuto = radio(Strings.t("orientation_auto"), 3)
            group.addView(optionPaysage)
            group.addView(optionPortrait)
            group.addView(optionAuto)
            when (currentOrientation) {
                "portrait" -> optionPortrait.isChecked = true
                "auto" -> optionAuto.isChecked = true
                else -> optionPaysage.isChecked = true
            }
            group.setOnCheckedChangeListener { _, checkedId ->
                val value = when (checkedId) {
                    optionPortrait.id -> "portrait"
                    optionAuto.id -> "auto"
                    else -> "landscape"
                }
                prefs.edit().putString(PicoloopActivity.PREF_ORIENTATION, value).apply()
            }
            addView(group)
        })

        content.addView(card {
            addView(rowTitle(Strings.t("labels_title")))
            addView(rowSubtitle(Strings.t("labels_subtitle")))
            val currentLabelMode = prefs.getString(OnScreenButtonOverlay.PREF_LABEL_MODE, "psp")
            val labelGroup = RadioGroup(this@SettingsActivity).apply { orientation = RadioGroup.VERTICAL }
            val optionPsp = radio(Strings.t("labels_psp"), 4)
            val optionPc = radio(Strings.t("labels_pc"), 5)
            labelGroup.addView(optionPsp)
            labelGroup.addView(optionPc)
            if (currentLabelMode == "pc") optionPc.isChecked = true else optionPsp.isChecked = true
            labelGroup.setOnCheckedChangeListener { _, checkedId ->
                val value = if (checkedId == optionPc.id) "pc" else "psp"
                prefs.edit().putString(OnScreenButtonOverlay.PREF_LABEL_MODE, value).apply()
            }
            addView(labelGroup)

            addView(spacer(12))
            addView(rowTitle(Strings.t("manual_title")))
            addView(rowSubtitle(Strings.t("manual_subtitle")))
            addView(outlinedButton(Strings.t("manual_view")) {
                val mode = prefs.getString(OnScreenButtonOverlay.PREF_LABEL_MODE, "psp") ?: "psp"
                startActivity(Intent(this@SettingsActivity, ManualActivity::class.java).putExtra(ManualActivity.EXTRA_MODE, mode))
            })
        })

        val storageFolder = try {
            NativeBridge.getStorageFolder()
        } catch (e: Exception) {
            MainActivity.PUBLIC_FOLDER_PATH
        }
        val iniSettings = readIniSettings(storageFolder)

        val folderInput = outlinedField(readStorageOverride(), MainActivity.PUBLIC_FOLDER_PATH)
        val keyRepeatInput = outlinedField(iniSettings.keyRepeat.toString(), null, InputType.TYPE_CLASS_NUMBER)
        val fontInput = outlinedField(iniSettings.font, "myfont.ttf")

        val themeKeys = listOf("theme_default", "theme_blue", "theme_autumn", "theme_grey", "theme_grey_nanoloop", "theme_yellow_nanoloop")
        val themeButtons = themeKeys.mapIndexed { i, key -> radio(Strings.t(key), 100 + i) }

        content.addView(card {
            addView(rowTitle(Strings.t("storage_title")))
            addView(rowSubtitle("${Strings.t("storage_current")}\n$storageFolder"))
            addView(spacer(8))
            addView(rowSubtitle(Strings.t("storage_folder_subtitle")))
            addView(folderInput)

            addView(spacer(16))
            addView(rowTitle(Strings.t("theme_title")))
            val themeGroup = RadioGroup(this@SettingsActivity).apply { orientation = RadioGroup.VERTICAL }
            themeButtons.forEach { themeGroup.addView(it) }
            if (iniSettings.theme in 0..5) themeButtons[iniSettings.theme].isChecked = true
            addView(themeGroup)

            addView(spacer(16))
            addView(rowTitle(Strings.t("keyrepeat_title")))
            addView(rowSubtitle(Strings.t("keyrepeat_subtitle")))
            addView(keyRepeatInput)

            addView(spacer(16))
            addView(rowTitle(Strings.t("font_title")))
            addView(rowSubtitle(Strings.t("font_subtitle")))
            addView(fontInput)

            addView(spacer(16))
            addView(filledButton(Strings.t("save_settings")) {
                writeStorageOverride(folderInput.text.toString().trim())
                val targetFolder = folderInput.text.toString().trim().ifBlank { MainActivity.PUBLIC_FOLDER_PATH }
                val checkedIndex = themeButtons.indexOfFirst { it.isChecked }
                writeIniSettings(
                    targetFolder,
                    IniSettings(
                        keyRepeat = keyRepeatInput.text.toString().toIntOrNull() ?: iniSettings.keyRepeat,
                        theme = checkedIndex,
                        font = fontInput.text.toString().trim()
                    )
                )
                Toast.makeText(this@SettingsActivity, Strings.t("save_settings_toast"), Toast.LENGTH_LONG).show()
            })
        })

        content.addView(card {
            addView(rowTitle(Strings.t("backups_title")))
            addView(rowSubtitle(Strings.t("backups_subtitle")))
            addView(filledButton(Strings.t("backups_create")) { createBackup(storageFolder) })
            addView(spacer(8))
            addView(outlinedButton(Strings.t("backups_restore")) { showRestoreDialog(storageFolder) })
        })

        content.addView(card(isLast = true) {
            addView(rowTitle(Strings.t("quit_title")))
            addView(rowSubtitle(Strings.t("quit_subtitle")))
            addView(dangerButton(Strings.t("quit_title")) {
                MaterialAlertDialogBuilder(this@SettingsActivity)
                    .setTitle(Strings.t("quit_confirm_title"))
                    .setMessage(Strings.t("quit_confirm_message"))
                    .setPositiveButton(Strings.t("quit_title")) { _, _ ->
                        NativeBridge.triggerQuit()
                        finishAffinity()
                    }
                    .setNegativeButton(Strings.t("cancel"), null)
                    .show()
            })
        })

        setContentView(scroll, ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT))
    }

    // ---- Material building blocks --------------------------------------

    private fun dp(v: Int) = (v * resources.displayMetrics.density).toInt()

    private fun themeColor(attr: Int): Int {
        val tv = android.util.TypedValue()
        theme.resolveAttribute(attr, tv, true)
        return tv.data
    }

    private fun card(isLast: Boolean = false, build: LinearLayout.() -> Unit): MaterialCardView {
        val inner = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(dp(20), dp(20), dp(20), dp(20))
        }
        inner.build()
        return MaterialCardView(this).apply {
            radius = dp(20).toFloat()
            cardElevation = 0f
            strokeWidth = dp(1)
            strokeColor = themeColor(com.google.android.material.R.attr.colorSurfaceVariant)
            setCardBackgroundColor(themeColor(com.google.android.material.R.attr.colorSurface))
            layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT).apply {
                bottomMargin = if (isLast) 0 else dp(14)
            }
            addView(inner)
        }
    }

    private fun pageTitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 26f
        typeface = Typeface.DEFAULT_BOLD
        setTextColor(themeColor(com.google.android.material.R.attr.colorOnSurface))
        setPadding(dp(4), 0, 0, dp(18))
    }

    private fun rowTitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 16f
        typeface = Typeface.DEFAULT_BOLD
        setTextColor(themeColor(com.google.android.material.R.attr.colorOnSurface))
    }

    private fun rowSubtitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 13f
        setTextColor(themeColor(com.google.android.material.R.attr.colorOnSurfaceVariant))
        setPadding(0, dp(3), 0, dp(8))
        setLineSpacing(dp(2).toFloat(), 1f)
    }

    private fun radio(label: String, buttonId: Int) = MaterialRadioButton(this).apply {
        text = label
        id = buttonId
        setPadding(0, dp(4), 0, dp(4))
    }

    private fun filledButton(label: String, onClick: () -> Unit) = MaterialButton(this).apply {
        text = label
        isAllCaps = false
        cornerRadius = dp(14)
        layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        setOnClickListener { onClick() }
    }

    private fun outlinedButton(label: String, onClick: () -> Unit) = MaterialButton(
        this, null, com.google.android.material.R.attr.materialButtonOutlinedStyle
    ).apply {
        text = label
        isAllCaps = false
        cornerRadius = dp(14)
        layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        setOnClickListener { onClick() }
    }

    private fun dangerButton(label: String, onClick: () -> Unit) = MaterialButton(
        this, null, com.google.android.material.R.attr.materialButtonOutlinedStyle
    ).apply {
        text = label
        isAllCaps = false
        cornerRadius = dp(14)
        val errorColor = themeColor(com.google.android.material.R.attr.colorError)
        setTextColor(errorColor)
        strokeColor = android.content.res.ColorStateList.valueOf(errorColor)
        layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
        setOnClickListener { onClick() }
    }

    // Plain EditText with a hand-drawn rounded outline, not TextInputLayout:
    // its box styling kept resolving back to the filled/underline look no
    // matter what was set programmatically (boxBackgroundMode/boxStrokeColor/
    // etc.) through this theme's default textInputStyle - this sidesteps
    // that entirely and is guaranteed to render the way it's told to.
    private fun outlinedField(initial: String, hint: String?, inputType: Int? = null): android.widget.EditText {
        return android.widget.EditText(this).apply {
            setText(initial)
            this.hint = hint
            if (inputType != null) setInputType(inputType)
            setTextColor(themeColor(com.google.android.material.R.attr.colorOnSurface))
            setHintTextColor(themeColor(com.google.android.material.R.attr.colorOnSurfaceVariant))
            background = android.graphics.drawable.GradientDrawable().apply {
                shape = android.graphics.drawable.GradientDrawable.RECTANGLE
                cornerRadius = dp(12).toFloat()
                setStroke(dp(1), themeColor(com.google.android.material.R.attr.colorOnSurfaceVariant))
                setColor(Color.TRANSPARENT)
            }
            setPadding(dp(14), dp(12), dp(14), dp(12))
            layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT).apply {
                topMargin = dp(4)
            }
        }
    }

    private fun spacer(heightDp: Int) = View(this).apply {
        layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, dp(heightDp))
    }

    // ---- Backups ---------------------------------------------------------

    private fun backupsDir(storageFolder: String): File {
        val dir = File(storageFolder, "backups")
        if (!dir.exists()) dir.mkdirs()
        return dir
    }

    private fun createBackup(storageFolder: String) {
        val bankDir = File(storageFolder, "bank")
        if (!bankDir.exists()) {
            Toast.makeText(this, Strings.t("backups_none_to_create"), Toast.LENGTH_SHORT).show()
            return
        }
        val stamp = SimpleDateFormat("yyyyMMdd_HHmmss", Locale.US).format(Date())
        val outFile = File(backupsDir(storageFolder), "picoloop_backup_$stamp.plsnap")
        try {
            ZipOutputStream(outFile.outputStream()).use { zip ->
                zipDirectory(bankDir, bankDir, zip)
            }
            Toast.makeText(this, "${Strings.t("backups_created")}\n${outFile.name}", Toast.LENGTH_LONG).show()
        } catch (e: Exception) {
            outFile.delete()
            Toast.makeText(this, "${Strings.t("backups_create_failed")} ${e.message}", Toast.LENGTH_LONG).show()
        }
    }

    private fun zipDirectory(root: File, current: File, zip: ZipOutputStream) {
        val files = current.listFiles() ?: return
        for (f in files) {
            val relPath = f.relativeTo(root).path
            if (f.isDirectory) {
                zipDirectory(root, f, zip)
            } else {
                zip.putNextEntry(ZipEntry(relPath))
                f.inputStream().use { it.copyTo(zip) }
                zip.closeEntry()
            }
        }
    }

    private fun showRestoreDialog(storageFolder: String) {
        val backups = backupsDir(storageFolder).listFiles { f -> f.name.endsWith(".plsnap") }
            ?.sortedByDescending { it.lastModified() } ?: emptyList()
        if (backups.isEmpty()) {
            Toast.makeText(this, Strings.t("backups_none_found"), Toast.LENGTH_SHORT).show()
            return
        }
        val labels = backups.map { it.name }.toTypedArray()
        MaterialAlertDialogBuilder(this)
            .setTitle(Strings.t("backups_restore"))
            .setItems(labels) { _, which -> confirmRestore(storageFolder, backups[which]) }
            .setNegativeButton(Strings.t("cancel"), null)
            .show()
    }

    private fun confirmRestore(storageFolder: String, backupFile: File) {
        MaterialAlertDialogBuilder(this)
            .setTitle("${Strings.t("backups_restore_confirm_title")} ${backupFile.name}?")
            .setMessage(Strings.t("backups_restore_confirm_message"))
            .setPositiveButton(Strings.t("backups_restore_confirm_title")) { _, _ -> restoreBackup(storageFolder, backupFile) }
            .setNegativeButton(Strings.t("cancel"), null)
            .show()
    }

    private fun restoreBackup(storageFolder: String, backupFile: File) {
        val bankDir = File(storageFolder, "bank")
        try {
            bankDir.deleteRecursively()
            bankDir.mkdirs()
            ZipInputStream(backupFile.inputStream()).use { zip ->
                var entry = zip.nextEntry
                while (entry != null) {
                    val outFile = File(bankDir, entry.name)
                    if (entry.isDirectory) {
                        outFile.mkdirs()
                    } else {
                        outFile.parentFile?.mkdirs()
                        outFile.outputStream().use { zip.copyTo(it) }
                    }
                    zip.closeEntry()
                    entry = zip.nextEntry
                }
            }
            // The native process still has the old state cached in memory -
            // a normal autosave-on-exit would clobber the files we just
            // restored, so close without triggering one (no NativeBridge
            // call needed here at all - nothing to save). Give the user a
            // moment to read that before the app closes.
            MaterialAlertDialogBuilder(this)
                .setTitle(Strings.t("backups_restore_done_title"))
                .setMessage(Strings.t("backups_restore_done_message"))
                .setCancelable(false)
                .setPositiveButton(Strings.t("ok")) { _, _ -> finishAffinity() }
                .show()
        } catch (e: Exception) {
            Toast.makeText(this, "${Strings.t("backups_restore_failed")} ${e.message}", Toast.LENGTH_LONG).show()
        }
    }

    // ---- picoloop.ini read/write ------------------------------------------
    // Mirrors picoloop/PicoloopIni.cpp's parser/format (key_repeat +
    // display sections). Kept intentionally simple/duplicated rather than
    // shared with native code - it's a handful of key=value lines. Only
    // "long" is exposed here (see keyrepeat_title in Strings.kt) -
    // smallest/small/middle/longest are left at their compiled defaults,
    // only settable by hand-editing the ini.
    private data class IniSettings(
        val keyRepeat: Int = 128,
        val theme: Int = -1,
        val font: String = ""
    )

    private fun readIniSettings(folder: String): IniSettings {
        var s = IniSettings()
        val f = File(folder, "picoloop.ini")
        if (!f.exists()) return s
        for (rawLine in f.readLines()) {
            val line = rawLine.trim()
            if (line.isEmpty() || line.startsWith("#") || line.startsWith(";") || line.startsWith("["))
                continue
            val idx = line.indexOf('=')
            if (idx < 0) continue
            val key = line.substring(0, idx).trim()
            val value = line.substring(idx + 1).trim()
            if (value.isEmpty()) continue
            when (key) {
                "long", "128" -> value.toIntOrNull()?.let { if (it > 0) s = s.copy(keyRepeat = it) }
                "font" -> s = s.copy(font = value)
                "theme" -> value.toIntOrNull()?.let { if (it in 0..5) s = s.copy(theme = it) }
            }
        }
        return s
    }

    private fun writeIniSettings(folder: String, s: IniSettings) {
        val dir = File(folder)
        if (!dir.exists()) dir.mkdirs()
        val sb = StringBuilder()
        sb.append("[key_repeat]\n")
        sb.append("long=${s.keyRepeat}\n")
        sb.append("\n[display]\n")
        if (s.font.isNotBlank()) sb.append("font=${s.font}\n")
        if (s.theme in 0..5) sb.append("theme=${s.theme}\n")
        File(dir, "picoloop.ini").writeText(sb.toString())
    }

    // The folder override lives in the app's private internal storage
    // (context.filesDir - same path SDL_AndroidGetInternalStoragePath()
    // resolves to natively), so SYSTEMANDROID.cpp can read it with a plain
    // fopen(), no JNI/SharedPreferences round trip.
    private fun readStorageOverride(): String {
        val f = File(filesDir, "storage_folder.txt")
        return if (f.exists()) f.readText().trim() else ""
    }

    private fun writeStorageOverride(path: String) {
        val f = File(filesDir, "storage_folder.txt")
        if (path.isBlank()) f.delete() else f.writeText(path)
    }
}
