package com.example.ece381;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.text.InputType;
import android.util.Log;
import android.widget.EditText;

public class TextEntryDialog extends DialogFragment implements OnClickListener {
	
    private EditText pl_name;

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        pl_name = new EditText(getActivity()); 
        pl_name.setInputType(InputType.TYPE_CLASS_TEXT); // QWERTY soft keyboard

        return new AlertDialog.Builder(getActivity())
        .setTitle(R.string.app_name)
        .setMessage("Name this playlist")
        .setPositiveButton("OK", this)
        .setNegativeButton("CANCEL", null)
        .setView(pl_name)
        .create();

    }

    //
    @Override
    public void onClick(DialogInterface dialog, int position) {

        String value = pl_name.getText().toString();
        Log.d("pl_name: ", value);
        
        // return value to PlaylistBuilder
        PlaylistBuilderActivity callingActivity = (PlaylistBuilderActivity) getActivity();
        callingActivity.onUserSelectValue(value);
        dialog.dismiss();
    }
}