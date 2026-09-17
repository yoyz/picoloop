package org.picoloop.android

import android.app.Activity
import android.content.Intent
import android.content.SharedPreferences
import android.graphics.Color
import android.os.Bundle
import android.view.Gravity
import android.view.ViewGroup
import android.widget.Button
import android.widget.LinearLayout
import android.widget.RadioButton
import android.widget.RadioGroup
import android.widget.ScrollView
import android.widget.TextView

/**
 * "picoloop options" screen. For now: on-screen button layout (position +
 * size, edited live over the game screen), the game screen's own position,
 * orientation, the PC/PSP button label style and its matching manual. More
 * entries can be appended to `content` later.
 */
class SettingsActivity : Activity() {
    private lateinit var prefs: SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        prefs = getSharedPreferences("picoloop_prefs", MODE_PRIVATE)

        val density = resources.displayMetrics.density
        fun dp(v: Int) = (v * density).toInt()

        val scroll = ScrollView(this)
        val content = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(dp(20), dp(24), dp(20), dp(24))
        }
        scroll.addView(content)

        content.addView(sectionTitle("Options picoloop"))

        content.addView(rowTitle("Disposition des boutons"))
        content.addView(rowSubtitle("Deplacer et agrandir/reduire les boutons a l'ecran"))
        content.addView(Button(this).apply {
            text = "Modifier la disposition"
            setOnClickListener {
                prefs.edit().putBoolean(PicoloopActivity.PREF_PENDING_EDIT_MODE, true).apply()
                finish()
            }
        })

        content.addView(spacer(dp(24)))

        content.addView(rowTitle("Position de l'ecran"))
        content.addView(rowSubtitle("Deplacer l'ecran du jeu (remonter en portrait, decaler sur le cote en paysage, etc.)"))
        content.addView(Button(this).apply {
            text = "Deplacer l'ecran"
            setOnClickListener {
                prefs.edit().putBoolean(PicoloopActivity.PREF_PENDING_SCREEN_EDIT_MODE, true).apply()
                finish()
            }
        })

        content.addView(spacer(dp(24)))

        content.addView(rowTitle("Orientation"))
        val currentOrientation = prefs.getString(PicoloopActivity.PREF_ORIENTATION, "landscape")
        val group = RadioGroup(this).apply { orientation = RadioGroup.VERTICAL }
        val optionPaysage = RadioButton(this).apply { text = "Paysage"; id = 1 }
        val optionPortrait = RadioButton(this).apply { text = "Portrait"; id = 2 }
        val optionAuto = RadioButton(this).apply { text = "Auto (suit la rotation de l'appareil)"; id = 3 }
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
        content.addView(group)

        content.addView(spacer(dp(24)))

        content.addView(rowTitle("Type d'affichage des boutons"))
        content.addView(rowSubtitle("Les noms utilises par picoloop dependent de la version du manuel : PC (Ctrl/Alt/Esc/Enter/Tab) ou PSP (A/B/Select/Start/L/R)"))
        val currentLabelMode = prefs.getString(OnScreenButtonOverlay.PREF_LABEL_MODE, "psp")
        val labelGroup = RadioGroup(this).apply { orientation = RadioGroup.VERTICAL }
        val optionPsp = RadioButton(this).apply { text = "PSP (A / B / Select / Start / L / R)"; id = 4 }
        val optionPc = RadioButton(this).apply { text = "PC (Ctrl / Alt / Esc / Enter / Tab)"; id = 5 }
        labelGroup.addView(optionPsp)
        labelGroup.addView(optionPc)
        if (currentLabelMode == "pc") optionPc.isChecked = true else optionPsp.isChecked = true
        labelGroup.setOnCheckedChangeListener { _, checkedId ->
            val value = if (checkedId == optionPc.id) "pc" else "psp"
            prefs.edit().putString(OnScreenButtonOverlay.PREF_LABEL_MODE, value).apply()
        }
        content.addView(labelGroup)

        content.addView(spacer(dp(24)))
        content.addView(rowTitle("Manuel"))
        content.addView(rowSubtitle("Le tutoriel complet de picoloop (correspondant au type de boutons choisi ci-dessus)"))
        content.addView(Button(this).apply {
            text = "Voir le manuel"
            setOnClickListener {
                val mode = prefs.getString(OnScreenButtonOverlay.PREF_LABEL_MODE, "psp") ?: "psp"
                startActivity(Intent(this@SettingsActivity, ManualActivity::class.java).putExtra(ManualActivity.EXTRA_MODE, mode))
            }
        })

        content.addView(spacer(dp(24)))
        content.addView(rowTitle("Stockage"))
        content.addView(rowSubtitle("Les banques picoloop sont sauvegardees dans :\n${MainActivity.PUBLIC_FOLDER_PATH}\n(ou dans le stockage prive de l'app si l'acces a ete refuse)"))

        setContentView(scroll, ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT))
    }

    private fun sectionTitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 22f
        setTextColor(Color.BLACK)
        setPadding(0, 0, 0, (16 * resources.displayMetrics.density).toInt())
    }

    private fun rowTitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 18f
        setTextColor(Color.BLACK)
    }

    private fun rowSubtitle(text: String) = TextView(this).apply {
        this.text = text
        textSize = 13f
        setTextColor(Color.DKGRAY)
        setPadding(0, (2 * resources.displayMetrics.density).toInt(), 0, (6 * resources.displayMetrics.density).toInt())
    }

    private fun spacer(heightPx: Int) = android.view.View(this).apply {
        layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, heightPx)
    }
}
