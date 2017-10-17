#!/bin/bash
sudo gcc ./src/main_cgi.c -o ./cgi-bin/main.cgi -std=c89 -Wall -Wextra -Werror -Wpedantic

sudo cp ./cgi-bin/* /var/www/cgi-bin
sudo cp ./html/* /var/www/html

