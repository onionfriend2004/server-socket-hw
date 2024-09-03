# server-socket-hw
# Задание
Написать сервер на сокетах, который отвечает ОК при подключении и закрывает соединение. Также написать клиент на сокете, который будет подключаться и проверять, что он получил ОК.
# Компиляция
Сервер
```c++
g++ socket-server.cpp -o socket-server
```
Клиент
```c++
g++ socket-client.cpp -o socket-client
```
# Запуск
Сервер
```c++
./socket-server SERVICE
```
Клиент
```c++
./socket-client NODE SERVICE
```
