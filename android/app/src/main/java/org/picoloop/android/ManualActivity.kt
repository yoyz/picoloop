package org.picoloop.android

import android.app.Activity
import android.graphics.Color
import android.graphics.Typeface
import android.os.Bundle
import android.text.SpannableStringBuilder
import android.text.style.RelativeSizeSpan
import android.text.style.StyleSpan
import android.widget.ScrollView
import android.widget.TextView

/**
 * Displays one of the two bundled picoloop tutorials (assets/picoloop_manual_pc.md
 * or _psp.md, fetched from https://github.com/farvardin/picoloop-manual) as
 * plain readable text. This is a small hand-rolled Markdown-ish renderer,
 * not a full parser: it handles exactly what these two files use (headers,
 * **bold**, <kbd>key</kbd> tags, pipe tables) and drops image references
 * (the manuals' screenshots aren't bundled with the app).
 */
class ManualActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val mode = intent.getStringExtra(EXTRA_MODE) ?: "psp"
        val assetName = if (mode == "pc") "picoloop_manual_pc.md" else "picoloop_manual_psp.md"
        val raw = try {
            assets.open(assetName).bufferedReader().use { it.readText() }
        } catch (e: Exception) {
            "Manuel indisponible (${e.message})."
        }

        val density = resources.displayMetrics.density
        fun dp(v: Int) = (v * density).toInt()

        val textView = TextView(this).apply {
            text = renderMarkdown(raw)
            setPadding(dp(20), dp(24), dp(20), dp(48))
            setTextColor(Color.BLACK)
            textSize = 15f
            setLineSpacing(4f, 1.15f)
        }
        val scroll = ScrollView(this).apply {
            setBackgroundColor(Color.WHITE)
            addView(textView)
        }
        setContentView(scroll)
    }

    private fun renderMarkdown(raw: String): SpannableStringBuilder {
        val out = SpannableStringBuilder()
        val kbdRegex = Regex("<kbd>(.*?)</kbd>")

        for (rawLine in raw.lineSequence()) {
            val line = rawLine.trimEnd()
            val trimmed = line.trim()

            if (trimmed.isEmpty()) continue
            if (trimmed.startsWith("![")) continue // image reference, no bundled screenshots
            if (trimmed.all { it == '-' || it == ' ' } && trimmed.length >= 3) continue // "---" rule

            val withoutKbd = kbdRegex.replace(line) { "[" + it.groupValues[1] + "]" }

            when {
                withoutKbd.startsWith("#### ") -> appendHeader(out, withoutKbd.removePrefix("#### "), 1.15f)
                withoutKbd.startsWith("### ") -> appendHeader(out, withoutKbd.removePrefix("### "), 1.3f)
                withoutKbd.startsWith("## ") -> appendHeader(out, withoutKbd.removePrefix("## "), 1.5f)
                withoutKbd.trim().startsWith("|") -> appendTableRow(out, withoutKbd)
                else -> appendParagraph(out, withoutKbd)
            }
        }
        return out
    }

    private fun appendHeader(out: SpannableStringBuilder, text: String, relativeSize: Float) {
        if (out.isNotEmpty()) out.append("\n\n")
        val start = out.length
        out.append(text.trim())
        out.setSpan(StyleSpan(Typeface.BOLD), start, out.length, 0)
        out.setSpan(RelativeSizeSpan(relativeSize), start, out.length, 0)
        out.append("\n")
    }

    private fun appendTableRow(out: SpannableStringBuilder, line: String) {
        if (out.isNotEmpty() && !out.endsWith("\n")) out.append("\n")
        val cells = line.trim().trim('|').split("|").map { it.trim() }
        if (cells.all { it.isEmpty() || it.all { c -> c == '-' } }) return // "|---|---|" separator row
        val start = out.length
        out.append(cells.joinToString("   -   "))
        out.setSpan(StyleSpan(Typeface.BOLD), start, out.length, 0)
        out.append("\n")
    }

    private fun appendParagraph(out: SpannableStringBuilder, line: String) {
        if (out.isNotEmpty()) out.append("\n\n")
        appendInlineBold(out, line.trim())
    }

    private fun appendInlineBold(out: SpannableStringBuilder, text: String) {
        val boldRegex = Regex("\\*\\*(.+?)\\*\\*")
        var last = 0
        for (match in boldRegex.findAll(text)) {
            out.append(text.substring(last, match.range.first))
            val start = out.length
            out.append(match.groupValues[1])
            out.setSpan(StyleSpan(Typeface.BOLD), start, out.length, 0)
            last = match.range.last + 1
        }
        out.append(text.substring(last))
    }

    companion object {
        const val EXTRA_MODE = "mode"
    }
}
