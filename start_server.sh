#!/bin/bash
sudo systemctl start mysql
sudo systemctl start redis-server
./chat_server