package com.example.client100;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final int REQUEST_ENABLE_BT = 2;

    private static final UUID MY_UUID = UUID.fromString("5779d4db-3e32-7c42-8959-951a52c114d8");
    public BluetoothAdapter mBluetoothAdapter;
    String code="FIRST";
    private BroadcastReceiver discoverDevicesReceiver;

    private BroadcastReceiver discoveryFinishedReceiver;
    private ProgressDialog progressDialog;

    private final List<BluetoothDevice> discoveredDevices = new ArrayList<BluetoothDevice>();
    private ArrayAdapter<BluetoothDevice> listAdapter;

    static public  ConnectThread thisDevices;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button buttontest=(Button)findViewById(R.id.buttonDisableBT);
               Button buttonWrite=(Button)findViewById(R.id.buttonEnableBT);
        ListView listDevices=(ListView)findViewById(R.id.listDevicesFound);

        buttontest.setOnClickListener(this);
        buttonWrite.setOnClickListener(this);


        
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            // Device does not support Bluetooth
            Toast.makeText(this, "No bluetooth supported", Toast.LENGTH_LONG).show();
            return;
        }
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }

        final ArrayAdapter<String> mArrayAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1);

        listAdapter = new ArrayAdapter<BluetoothDevice>(getBaseContext(), android.R.layout.simple_list_item_1, discoveredDevices) {
            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                View view = super.getView(position, convertView, parent);
                final BluetoothDevice device = getItem(position);
                ((TextView) view.findViewById(android.R.id.text1)).setText(device.getName()+"\n"+device.getAddress());
                return view;
            }
        };
       listDevices.setAdapter(listAdapter);
       listDevices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
           @Override
           public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
               thisDevices=new ConnectThread(discoveredDevices.get(position));
               new Thread(thisDevices).start();


           }
       });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.buttonDisableBT:
                Intent intent = new Intent(this, MouseActivity.class);
                startActivity(intent);
                code="aimp";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
                break;
            case R.id.buttonEnableBT:
                Intent intent1 = new Intent(this, ActivityAim.class);
                startActivity(intent1);
                break;


            default:
                break;
        }
    }



    public class ConnectThread extends Thread
    {
        private BluetoothSocket mmSocket = null;
        private OutputStream mOutput;

        public ConnectThread(BluetoothDevice device) {

            try {
                mmSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            // Cancel discovery because it will slow down the connection
            mBluetoothAdapter.cancelDiscovery();

            try {
                // Connect the device through the socket. This will block
                // until it succeeds or throws an exception
                mmSocket.connect();
            } catch (IOException connectException) {
                // Unable to connect; close the socket and get out
                try {
                    mmSocket.close();
                } catch (IOException closeException) {
                    closeException.printStackTrace();
                }
                connectException.printStackTrace();
                return;
            }

            try {
                mOutput = mmSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }

            // Packet sent over the bluetooth connection.
            // The server expects a string of 10+6+6 chars for this test.
            class PacketTest {
                char header[] = new char[10];
                char x[] = new char[6];
                char y[] = new char[6];

                public PacketTest(String s, int x, int y) {
                    for (int i = 0; i < 10 && i < s.length(); ++i) {
                        header[i] = s.charAt(i);
                    }
                    final String sx = String.valueOf(x);
                    for (int i = 0; i < 6 && i < sx.length(); ++i) {
                        this.x[i] = sx.charAt(i);
                    }
                    final String sy = String.valueOf(y);
                    for (int i = 0; i < 6 && i < sy.length(); ++i) {
                        this.y[i] = sy.charAt(i);
                    }
                }

                public String toString() {
                    StringBuilder sb = new StringBuilder(10 + 6 + 6);
                    sb.append(header);
                    sb.append(x);
                    sb.append(y);
                    return sb.toString();
                }
            }

            /*for (int x = 0; x < 10; ++x) {
                for (int y = 0; y < 10; ++y) {
                    PacketTest p = new PacketTest("Hello", x, y);

                    final String str = p.toString();
                    Log.d("TEST", String.format("Gonna write [%s]", str));
                    // Send to the server !
                    write(str.getBytes());
                }
            }*/
            //cancel();
        }

        public void write(byte[] bytes) {
            try {
                mOutput.write(bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void discoverDevices(View view) {

        discoveredDevices.clear();
        listAdapter.notifyDataSetChanged();

        if (discoverDevicesReceiver == null) {
            discoverDevicesReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    String action = intent.getAction();

                    if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

                        if (!discoveredDevices.contains(device)) {
                            discoveredDevices.add(device);
                            listAdapter.notifyDataSetChanged();
                        }
                    }
                }
            };
        }
        if (discoveryFinishedReceiver == null) {
            discoveryFinishedReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    //getListView().setEnabled(true);
                    if (progressDialog != null)
                        progressDialog.dismiss();
                    Toast.makeText(getBaseContext(), "Поиск закончен. Выберите устройство для отправки  cообщения.", Toast.LENGTH_LONG).show();
                    unregisterReceiver(discoveryFinishedReceiver);
                }
            };
        }

        registerReceiver(discoverDevicesReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        registerReceiver(discoveryFinishedReceiver, new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED));

        progressDialog = ProgressDialog.show(this, "Поиск устройств", "Подождите...");
        mBluetoothAdapter.startDiscovery();
    }


}
