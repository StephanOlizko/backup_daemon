# Project Title

This project contains a backup daemon written in C++. The daemon monitors a specified directory and backs up any new or modified files to a backup directory.

## Files

- [`backup_config.conf`](backup_config.conf): This is the configuration file for the backup daemon. It specifies the source directory to monitor and the backup directory where backups are stored.
- [`backup_daemon.cpp`](backup_daemon.cpp): This is the source code for the backup daemon. It uses the inotify API to monitor the source directory for changes.
- [`backup_daemon.service`](backup_daemon.service): This is the systemd service file for the backup daemon. It allows the daemon to be managed by systemd, enabling it to start on boot and be restarted if it crashes.
- [`source_directory/`](source_directory/): This directory contains files that are backed up by the daemon.

## Usage

To use the backup daemon, follow these steps:

1. Compile the C++ source code with a C++ compiler that supports the C++11 standard or later. For example, you can use g++: `g++ -std=c++11 backup_daemon.cpp -o backup_daemon`
2. Install the service file to your systemd directory. This is usually `/etc/systemd/system/`. You might need root permissions to do this.
3. Adjust the configuration file to your needs. Specify the source directory to monitor and the backup directory where backups should be stored.
4. Start the service with `systemctl start backup_daemon.service`. You can enable it to start on boot with `systemctl enable backup_daemon.service`.
