package org.picoloop.android

import android.content.Context
import android.content.SharedPreferences

/**
 * Tiny hand-rolled bilingual lookup for the app's Kotlin-side UI text
 * (Settings screen, edit-mode bar, first-run folder toast, manual-missing
 * fallback). Not Android's resource system (strings.xml/values-fr) because
 * the UI is built entirely in code, not from layout resources, and this
 * needs to be switchable from inside the app (Settings), not just follow
 * the system locale. Backed by the same "picoloop_prefs" SharedPreferences
 * used elsewhere (PicoloopActivity/SettingsActivity), English by default.
 */
object Strings {
    private const val PREF_LANGUAGE = "language"
    private lateinit var prefs: SharedPreferences

    fun init(context: Context) {
        if (!::prefs.isInitialized) {
            prefs = context.getSharedPreferences("picoloop_prefs", Context.MODE_PRIVATE)
        }
    }

    var language: String
        get() = prefs.getString(PREF_LANGUAGE, "en") ?: "en"
        set(value) { prefs.edit().putString(PREF_LANGUAGE, value).apply() }

    fun t(key: String): String = table[key]?.get(language) ?: table[key]?.get("en") ?: key

    private val table: Map<String, Map<String, String>> = mapOf(
        "options_title" to mapOf("en" to "picoloop options", "fr" to "Options picoloop"),

        "language_title" to mapOf("en" to "Language", "fr" to "Langue"),
        "language_subtitle" to mapOf("en" to "Interface language", "fr" to "Langue de l'interface"),

        "layout_title" to mapOf("en" to "Button layout", "fr" to "Disposition des boutons"),
        "layout_subtitle" to mapOf("en" to "Move and resize the on-screen buttons", "fr" to "Deplacer et agrandir/reduire les boutons a l'ecran"),
        "layout_edit" to mapOf("en" to "Edit layout", "fr" to "Modifier la disposition"),

        "screen_pos_title" to mapOf("en" to "Screen position", "fr" to "Position de l'ecran"),
        "screen_pos_subtitle" to mapOf("en" to "Move the game screen (shift up in portrait, sideways in landscape, etc.)", "fr" to "Deplacer l'ecran du jeu (remonter en portrait, decaler sur le cote en paysage, etc.)"),
        "screen_pos_edit" to mapOf("en" to "Move screen", "fr" to "Deplacer l'ecran"),

        "orientation_title" to mapOf("en" to "Orientation", "fr" to "Orientation"),
        "orientation_landscape" to mapOf("en" to "Landscape", "fr" to "Paysage"),
        "orientation_portrait" to mapOf("en" to "Portrait", "fr" to "Portrait"),
        "orientation_auto" to mapOf("en" to "Auto (follows device rotation)", "fr" to "Auto (suit la rotation de l'appareil)"),

        "labels_title" to mapOf("en" to "Button label style", "fr" to "Type d'affichage des boutons"),
        "labels_subtitle" to mapOf("en" to "The names picoloop uses depend on the manual version: PC (Ctrl/Alt/Esc/Enter/Tab) or PSP (A/B/Select/Start/L/R)", "fr" to "Les noms utilises par picoloop dependent de la version du manuel : PC (Ctrl/Alt/Esc/Enter/Tab) ou PSP (A/B/Select/Start/L/R)"),
        "labels_psp" to mapOf("en" to "PSP (A / B / Select / Start / L / R)", "fr" to "PSP (A / B / Select / Start / L / R)"),
        "labels_pc" to mapOf("en" to "PC (Ctrl / Alt / Esc / Enter / Tab)", "fr" to "PC (Ctrl / Alt / Esc / Enter / Tab)"),

        "manual_title" to mapOf("en" to "Manual", "fr" to "Manuel"),
        "manual_subtitle" to mapOf("en" to "The full picoloop tutorial (matching the button type chosen above)", "fr" to "Le tutoriel complet de picoloop (correspondant au type de boutons choisi ci-dessus)"),
        "manual_view" to mapOf("en" to "View manual", "fr" to "Voir le manuel"),
        "manual_unavailable" to mapOf("en" to "Manual unavailable", "fr" to "Manuel indisponible"),

        "storage_title" to mapOf("en" to "Storage", "fr" to "Stockage"),
        "storage_current" to mapOf("en" to "picoloop banks are currently saved in:", "fr" to "Les banques picoloop sont actuellement sauvegardees dans :"),
        "storage_folder_subtitle" to mapOf("en" to "Custom folder (absolute path, empty = default folder). Requires restarting picoloop to take effect.", "fr" to "Dossier personnalise (chemin absolu, vide = dossier par defaut). Necessite de redemarrer picoloop pour prendre effet."),

        "theme_title" to mapOf("en" to "Default theme", "fr" to "Theme par defaut"),
        "theme_default" to mapOf("en" to "Default", "fr" to "Defaut"),
        "theme_blue" to mapOf("en" to "Blue", "fr" to "Bleu"),
        "theme_autumn" to mapOf("en" to "Autumn", "fr" to "Automne"),
        "theme_grey" to mapOf("en" to "Grey", "fr" to "Gris"),
        "theme_grey_nanoloop" to mapOf("en" to "Grey nanoloop", "fr" to "Gris nanoloop"),
        "theme_yellow_nanoloop" to mapOf("en" to "Yellow gameboy nanoloop", "fr" to "Jaune gameboy nanoloop"),

        "keyrepeat_title" to mapOf("en" to "Key repeat speed", "fr" to "Vitesse de repetition des touches"),
        "keyrepeat_subtitle" to mapOf("en" to "Ticks between repeats while a key is held (bigger = slower). A tick is one main-loop iteration, roughly 1ms.", "fr" to "Ticks entre deux repetitions pendant qu'une touche est maintenue (plus grand = plus lent). Un tick correspond a un tour de la boucle principale, environ 1ms."),

        "font_title" to mapOf("en" to "Font", "fr" to "Police"),
        "font_subtitle" to mapOf("en" to "Path to a .ttf to use instead of the default font (empty = default font).", "fr" to "Chemin vers un .ttf a utiliser a la place de la police par defaut (vide = police par defaut)."),

        "save_settings" to mapOf("en" to "Save these settings", "fr" to "Enregistrer ces reglages"),
        "save_settings_toast" to mapOf("en" to "Saved. Restart picoloop to apply.", "fr" to "Enregistre. Redemarre picoloop pour appliquer."),

        "backups_title" to mapOf("en" to "Backups", "fr" to "Sauvegardes"),
        "backups_subtitle" to mapOf("en" to "Creates a full copy of all banks, restorable later to roll back to an earlier state.", "fr" to "Cree une copie complete de toutes les banques, a restaurer en cas de besoin pour revenir a un etat anterieur."),
        "backups_create" to mapOf("en" to "Create a backup", "fr" to "Creer une sauvegarde"),
        "backups_restore" to mapOf("en" to "Restore a backup", "fr" to "Restaurer une sauvegarde"),
        "backups_none_to_create" to mapOf("en" to "No banks to back up", "fr" to "Aucune banque a sauvegarder"),
        "backups_created" to mapOf("en" to "Backup created:", "fr" to "Sauvegarde creee :"),
        "backups_create_failed" to mapOf("en" to "Backup failed:", "fr" to "Echec de la sauvegarde :"),
        "backups_none_found" to mapOf("en" to "No backups found", "fr" to "Aucune sauvegarde trouvee"),
        "backups_restore_confirm_title" to mapOf("en" to "Restore", "fr" to "Restaurer"),
        "backups_restore_confirm_message" to mapOf("en" to "Current banks will be replaced by this backup's. This action is irreversible.", "fr" to "Les banques actuelles seront remplacees par celles de cette sauvegarde. Cette action est irreversible."),
        "backups_restore_done_title" to mapOf("en" to "Backup restored", "fr" to "Sauvegarde restauree"),
        "backups_restore_done_message" to mapOf("en" to "picoloop will close. Reopen the app to see the restored banks.", "fr" to "picoloop va se fermer. Rouvre l'app pour voir les banques restaurees."),
        "backups_restore_failed" to mapOf("en" to "Restore failed:", "fr" to "Echec de la restauration :"),

        "quit_title" to mapOf("en" to "Quit", "fr" to "Quitter"),
        "quit_subtitle" to mapOf("en" to "Closes picoloop cleanly (saves the current state before closing).", "fr" to "Ferme picoloop proprement (sauvegarde l'etat en cours avant de fermer)."),
        "quit_confirm_title" to mapOf("en" to "Quit picoloop?", "fr" to "Quitter picoloop ?"),
        "quit_confirm_message" to mapOf("en" to "The current state will be saved before closing.", "fr" to "L'etat en cours sera sauvegarde avant fermeture."),

        "cancel" to mapOf("en" to "Cancel", "fr" to "Annuler"),
        "ok" to mapOf("en" to "OK", "fr" to "OK"),

        "editbar_minus" to mapOf("en" to "-", "fr" to "-"),
        "editbar_plus" to mapOf("en" to "+", "fr" to "+"),
        "editbar_reset" to mapOf("en" to "Reset", "fr" to "Reinitialiser"),
        "editbar_done" to mapOf("en" to "Done", "fr" to "Termine"),

        "public_folder_created" to mapOf("en" to "Folder created:", "fr" to "Dossier cree :"),
        "public_folder_created_suffix" to mapOf("en" to "(picoloop banks will be saved there)", "fr" to "(les banques picoloop y seront sauvegardees)")
    )
}
