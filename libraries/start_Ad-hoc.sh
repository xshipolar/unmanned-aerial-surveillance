#!/bin/bash
#######

#starts the ad hoc server
iwconfig wlan0 mode ad-hoc
iwconfig wlan0 channel 4
iwconfig wlan0 essid 'yyy'
ifconfig ath0 10.0.0.1 up
