## 🛜 ezBLE
`ezBLE` is an Arduino library which makes sending and receiving data over BLE super simple. It provides the same API as `Serial` and handles connections, discovery, data transfer, and all BLE tasks in the background. It's compatible with all Silicon Labs BLE hardware.

You can use it the same way as Serial to transfer data over BLE. You'll need at least two devices, one acting as a server and one as a client. Once the connection is established the communication is bidirectional, each device can send and receive data from each other over BLE.

Initialize it as client or server:
```
ezBLE.beginClient()
```

or
```
ezBLE.beginServer()
```

You can optionally name your ezBLE nodes to separate them from others. The client and the server will both look for the configured name and connect to each other:
```
ezBLE.beginClient("myCustomEzBleName")
```
```
ezBLE.beginServer("myCustomEzBleName")
```


Wait for the connection to establish:
```
while(!ezBLE.connected());
```

Send data:
```
ezBLE.println("Hello, BLE!")
```

Receive data:
```
while(ezBLE.available()) {
  Serial.print((char)ezBLE.read());
}
```

The `ezBLE` library is shipped with the core, no need to download it separately. See the included examples for full code.
