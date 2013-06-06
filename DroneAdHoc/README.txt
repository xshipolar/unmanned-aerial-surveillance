To establish an ad-hoc network on startup:
Place the .service file under /etc/systemd/system/ and modifiy the ExecStart path to point to the location of the AdHocScript file.
Then give the AdHocScript file exec permisions by entering "chmod 755 AdHocScript" in the command line.
The SetUpAdHoc script can be used on the control computer to connect to the drone.
