package com.zarubond.atlas;

import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.KeyEvent;
import java.lang.Math;
import java.math.BigDecimal;

public class GamePad {

    private float axis_x=0.0f, axis_y=0.0f, axis_rx=0.0f, axis_ry=0.0f;

    public void motionEvent(MotionEvent event) {

        if(axis_x != event.getAxisValue(MotionEvent.AXIS_X))
            gamepadAxis(0, round(event.getAxisValue (MotionEvent.AXIS_X),2));
        axis_x=event.getAxisValue(MotionEvent.AXIS_X);

        if(axis_y != event.getAxisValue(MotionEvent.AXIS_Y))
            gamepadAxis(1, round(event.getAxisValue (MotionEvent.AXIS_Y),2));
        axis_y=event.getAxisValue(MotionEvent.AXIS_Y);

        if(axis_rx != event.getAxisValue(MotionEvent.AXIS_RX))
            gamepadAxis(2, round(event.getAxisValue (MotionEvent.AXIS_RX),2));
        axis_rx=event.getAxisValue(MotionEvent.AXIS_RX);

        System.out.println(event.getAxisValue(MotionEvent.AXIS_RZ));

        if(axis_ry != event.getAxisValue(MotionEvent.AXIS_RZ))
            gamepadAxis(3, round(event.getAxisValue (MotionEvent.AXIS_RZ),2));
        axis_ry=event.getAxisValue(MotionEvent.AXIS_RZ);

    }

    public void keyEvent(KeyEvent event) {
        System.out.println(event);
        if(event.getAction()==KeyEvent.ACTION_DOWN)
        {
            gamepadButton(event.getKeyCode(), true);
        }
        else if(event.getAction()==KeyEvent.ACTION_UP)
        {
            gamepadButton(event.getKeyCode(), false);
        }
    }

    public static float round(float d, int decimalPlace) {
        BigDecimal bd = new BigDecimal(Float.toString(d));
        bd = bd.setScale(decimalPlace, BigDecimal.ROUND_HALF_UP);
        return bd.floatValue();
    }

    public static native void gamepadAxis(int axes, float value);
    public static native void gamepadButton(int button, boolean press);
}



