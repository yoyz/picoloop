package org.picoloop.android

import android.Manifest
import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.util.Log
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.io.File

/**
 * Launcher activity: requests "all files access" so picoloop can read/write
 * its bank/ folder from a public, user-browsable location instead of the
 * app-private sandbox, then hands off to PicoloopActivity (the actual SDL
 * activity). Same approach as LittlePiggyTracker's MainActivity.
 *
 * SYSTEMANDROID.cpp falls back to app-private storage on its own if this
 * permission ends up denied, so the app is always usable either way - this
 * activity only tries to make the nicer, user-visible location available.
 */
class MainActivity : Activity() {
    companion object {
        private const val TAG = "PicoloopMainActivity"
        private const val STORAGE_PERMISSION_CODE = 100
        const val PUBLIC_FOLDER_PATH = "/storage/emulated/0/picoloop"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Strings.init(this)
        requestStoragePermissionThenStart()
    }

    private fun requestStoragePermissionThenStart() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if (!Environment.isExternalStorageManager()) {
                Log.i(TAG, "Requesting MANAGE_EXTERNAL_STORAGE via Settings")
                try {
                    val intent = Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION)
                    intent.data = Uri.parse("package:$packageName")
                    startActivityForResult(intent, STORAGE_PERMISSION_CODE)
                } catch (e: Exception) {
                    startActivityForResult(Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION), STORAGE_PERMISSION_CODE)
                }
                return
            }
            createPublicFolder()
            startPicoloopActivity()
        } else {
            val permissions = arrayOf(
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            )
            val needsRequest = permissions.any {
                ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
            }
            if (needsRequest) {
                ActivityCompat.requestPermissions(this, permissions, STORAGE_PERMISSION_CODE)
            } else {
                createPublicFolder()
                startPicoloopActivity()
            }
        }
    }

    private fun createPublicFolder() {
        try {
            val publicFolder = File(PUBLIC_FOLDER_PATH)
            if (!publicFolder.exists()) {
                val created = publicFolder.mkdirs()
                Log.i(TAG, "picoloop public folder: created=$created path=${publicFolder.absolutePath}")
                if (created) {
                    Toast.makeText(
                        this,
                        "${Strings.t("public_folder_created")} ${publicFolder.absolutePath}\n${Strings.t("public_folder_created_suffix")}",
                        Toast.LENGTH_LONG
                    ).show()
                }
            }
        } catch (e: Exception) {
            Log.e(TAG, "Failed to create public picoloop folder", e)
        }
    }

    @Deprecated("Deprecated in Java")
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == STORAGE_PERMISSION_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R && Environment.isExternalStorageManager()) {
                Log.i(TAG, "MANAGE_EXTERNAL_STORAGE granted")
                createPublicFolder()
            } else {
                Log.w(TAG, "MANAGE_EXTERNAL_STORAGE denied - using app-private folder only")
            }
            startPicoloopActivity()
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == STORAGE_PERMISSION_CODE) {
            if (grantResults.isNotEmpty() && grantResults.all { it == PackageManager.PERMISSION_GRANTED }) {
                createPublicFolder()
            } else {
                Log.w(TAG, "Storage permissions denied - using app-private folder only")
            }
            startPicoloopActivity()
        }
    }

    private fun startPicoloopActivity() {
        startActivity(Intent(this, PicoloopActivity::class.java))
        finish()
    }
}
