package com.zarubond.atlas;

import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.app.Activity;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.hardware.input.InputManager;

public class AtlasActivity extends org.qtproject.qt5.android.bindings.QtActivity{

    private GamePad gamepad=new GamePad();
    public AtlasActivity() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    @Override
    public boolean dispatchGenericMotionEvent(MotionEvent event) {
        if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK && event.getAction() == MotionEvent.ACTION_MOVE)
            gamepad.motionEvent(event);

        return super.dispatchGenericMotionEvent(event);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {

        if ((event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD)
            gamepad.keyEvent(event);
        return super.dispatchKeyEvent(event);
    }

}
