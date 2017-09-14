package com.example.root.controller;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import static java.lang.Math.abs;


public class MainActivity extends AppCompatActivity {
    private static SeekBar seek_bar1;
    private static TextView text_view1;
    private static SeekBar seek_bar2;
    private static TextView text_view2;

    private static String seek1Out = "<+000";
    private static String seek2Out = "l000>";

    private final String DEVICE_NAME = "HC-05";
    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");//Serial Port Service ID
    private BluetoothDevice device;
    private BluetoothSocket socket;
    private OutputStream outputStream;
    private InputStream inputStream;
    boolean deviceConnected = false;
    Button startButton;
    TextView textView1,textView2,textView3;
    SeekBar seek1, seek2;
    Thread thread;
    byte buffer[];
    int bufferPosition;
    boolean stopThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startButton = (Button) findViewById(R.id.buttonStart);
        textView1 = (TextView) findViewById(R.id.textView1);
        textView2 = (TextView) findViewById(R.id.textView2);
        textView3 = (TextView) findViewById(R.id.textView3);
        seek1 = (SeekBar) findViewById(R.id.seekBar1);
        seek2 = (SeekBar) findViewById(R.id.seekBar2);
        setUiEnabled(false);
    }

    // Code for seek Bars
    public void seekbar1() {
        seek_bar1 = (SeekBar) findViewById(R.id.seekBar1);
        text_view1 = (TextView) findViewById(R.id.textView1);
        seek_bar1.setProgress(50);
        text_view1.setText(String.valueOf(seek_bar1.getProgress() - 50));

        seek_bar1.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    int progress_value;
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                        progress_value = progress - 50;
                        //evaluating string to be sent
                        int scaledVal = abs(10*progress_value);
                        int ones = (scaledVal % 10);
                        int tens = ((scaledVal % 100) - ones)/10;
                        int hun = scaledVal/100;
                        String string = String.valueOf(hun)+String.valueOf(tens)+String.valueOf(ones);
                        if (progress_value >= 0){
                            string = "<+" + string;
                        }else{
                            string = "<-" + string;
                        }
                        //String evaluation ends
                        seek1Out = string;
                        send();
                        text_view1.setText(String.valueOf(progress_value));
                        Toast.makeText(MainActivity.this, "seekBar1 in progress", Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                        Toast.makeText(MainActivity.this, "seekBar1 in start", Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        text_view1.setText(String.valueOf(progress_value));
                        Toast.makeText(MainActivity.this, "seekBar1 in stop", Toast.LENGTH_LONG).show();
                    }
                }
        );
    }

    public void seekbar2() {
        seek_bar2 = (SeekBar) findViewById(R.id.seekBar2);
        text_view2 = (TextView) findViewById(R.id.textView2);
        seek_bar2.setProgress(50);
        text_view2.setText(String.valueOf(seek_bar2.getProgress() - 50));

        seek_bar2.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    int progress_value;
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                        progress_value = progress - 50;

                        //evaluating string to be sent
                        int scaledVal = abs(10*progress_value);
                        int ones = (scaledVal % 10);
                        int tens = ((scaledVal % 100) - ones)/10;
                        int hun = scaledVal/100;
                        String string = String.valueOf(hun)+String.valueOf(tens)+String.valueOf(ones);
                        if (progress_value >= 0){
                            string = "l" + string + ">";
                        }else{
                            string = "r" + string + ">";
                        }
                        //String evaluation ends

                        seek2Out = string;
                        send();
                        text_view2.setText(String.valueOf(progress_value));
                        Toast.makeText(MainActivity.this, "seekBar2 in progress", Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                        Toast.makeText(MainActivity.this, "seekBar2 in start", Toast.LENGTH_LONG).show();
                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        text_view2.setText(String.valueOf(progress_value));
                        Toast.makeText(MainActivity.this, "seekBar2 in stop", Toast.LENGTH_LONG).show();
                    }
                }
        );
    }

    // Enable/disable UI function
    public void setUiEnabled(boolean bool){

        startButton.setEnabled(!bool);
        textView1.setEnabled(bool);
        textView2.setEnabled(bool);
        textView3.setEnabled(bool);
        seek1.setEnabled(bool);
        seek2.setEnabled(bool);
    }

    // Bluetooth Initializing Function
    public boolean BTinit(){
        boolean found = false;
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null){
            Toast.makeText(getApplicationContext(),"Device doesn't support bluetooth",Toast.LENGTH_SHORT).show();
        }
        if (!bluetoothAdapter.isEnabled()){
            Intent enableAdapter = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableAdapter,0);
            try{
                Thread.sleep(1000);
            }catch (InterruptedException e){
                e.printStackTrace();
            }
        }

        Set<BluetoothDevice> bondedDevices = bluetoothAdapter.getBondedDevices();
        if (bondedDevices.isEmpty()){
            Toast.makeText(getApplicationContext(),"Please pair the device first",Toast.LENGTH_SHORT).show();
        }else{
            for (BluetoothDevice iterator : bondedDevices){
                if (iterator.getName().equals(DEVICE_NAME)){
                    device = iterator;
                    found = true;
                    break;
                }
            }
        }

        return found;
    }

    // Connect with bluetooth function
    public  boolean BTconnect(){
        boolean connected = true;
        try{
            socket = device.createRfcommSocketToServiceRecord(PORT_UUID);
            socket.connect();
        }catch (IOException e){
            e.printStackTrace();
            connected = false;
        }

        if (connected){
            try{
                outputStream = socket.getOutputStream();
            }catch (IOException e){
                e.printStackTrace();
            }

            try{
                inputStream = socket.getInputStream();
            }catch (IOException e){
                e.printStackTrace();
            }
        }
        return connected;
    }

    // Start Button
    public void onClickStart(View view){
        if (BTinit()){
            if (BTconnect()){
                setUiEnabled(true);
                deviceConnected = true;
                seekbar1();
                seekbar2();
                beginListenForData();
            }
        }
    }

    // Listen for data
    void beginListenForData(){
        final Handler handler = new Handler();
        stopThread = false;
        buffer = new  byte[1024];
        Thread thread = new Thread(
                new Runnable() {
                    @Override
                    public void run() {
                        while (!Thread.currentThread().isInterrupted() && !stopThread){
                            try{
                                int byteCount = inputStream.available();
                                if (byteCount > 0){
                                    byte [] rawBytes = new byte[byteCount];
                                    inputStream.read(rawBytes);
                                    final String string = new String(rawBytes,"UTF-8");
                                    handler.post(new Runnable() {
                                        public void run(){textView3.append(string);}
                                    });
                                }
                            }catch (IOException ex){
                                stopThread = true;
                            }
                        }
                    }
                });
        thread.start();
    }


    // Send Function
    public void send(){

        String out = seek1Out+seek2Out;
        try{
            outputStream.write(out.getBytes());
        }catch (IOException e){
            e.printStackTrace();
        }
    }
}
