# Define "version" of boeffla tweaks and dialog apps coming with this kernel
# and the file where the information gets stored
BT_VERSION_NEW="001"
BT_VERSION_FILE="/data/boeffla-appversion"

# Uninstall boeffla-tweak and dialog helper apps that might still be installed in system/app
if [ -f /system/app/BoefflaTweaks.apk ]; then
	/sbin/busybox mount -o remount,rw -t ext4 $SYSTEM_DEVICE /system
	/sbin/busybox rm /system/app/BoefflaTweaks.apk
	/sbin/busybox mount -o remount,ro -t ext4 $SYSTEM_DEVICE /system
	echo $(date) Boeffla tweaks app removed from system/app folder >> $BOEFFLA_LOGFILE
fi

if [ -f /system/app/BoefflaTweaksDialogHelper.apk ]; then
	/sbin/busybox mount -o remount,rw -t ext4 $SYSTEM_DEVICE /system
	/sbin/busybox rm /system/app/BoefflaTweaksDialogHelper.apk
	/sbin/busybox mount -o remount,ro -t ext4 $SYSTEM_DEVICE /system
	echo $(date) Boeffla dialog app removed from system/app folder >> $BOEFFLA_LOGFILE
fi


# Depending on the config setting, boeffla tweaks and boeffla dialog apps
# get installed/upgraded or uninstalled

if /sbin/busybox [ "`/sbin/busybox grep boeffla_app=off $BOEFFLA_CONFIG`" ]; then

	# If app is installed, uninstall it now and delete all config scripts
	STATUS=`/system/bin/pm path bo.boeffla`
	if [ ! -z $STATUS ]; then
		/system/bin/pm uninstall bo.boeffla

		/sbin/busybox mount -o remount,rw -t ext4 $SYSTEM_DEVICE /system
		/sbin/busybox rm /system/etc/init.d/*_bk*
		/sbin/busybox mount -o remount,ro -t ext4 $SYSTEM_DEVICE /system

		echo $(date) Boeffla tweaks app uninstalled and settings deleted >> $BOEFFLA_LOGFILE
	fi

	# If dialog app is installed, uninstall it now
	STATUS=`/system/bin/pm path bo.boeffla.tweaks.dialog.helper`
	if [ ! -z $STATUS ]; then
		/system/bin/pm uninstall bo.boeffla.tweaks.dialog.helper
		echo $(date) Boeffla dialog app uninstalled >> $BOEFFLA_LOGFILE
	fi

else

	# Retrieve "version information" of current version file
	BT_VERSION_CURRENT=`/sbin/busybox cat $BT_VERSION_FILE`

	# Get current installation status of the apps
	STATUS1=`/system/bin/pm path bo.boeffla`
	STATUS2=`/system/bin/pm path bo.boeffla.tweaks.dialog.helper`

	# If one of the apps is not installed or an update is enforced, install/update them now
	if [ -z $STATUS1 ] || [ -z $STATUS2 ] || [ ${BT_VERSION_CURRENT:-0} != $BT_VERSION_NEW ]; then

		/system/bin/pm install -r /res/app/BoefflaTweaks.apk
		/system/bin/pm install -r /res/app/BoefflaTweaksDialogHelper.apk

		echo $BT_VERSION_NEW > $BT_VERSION_FILE

		echo $(date) Boeffla tweaks and dialog apps installed/updated >> $BOEFFLA_LOGFILE
	fi

fi
