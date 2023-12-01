#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <syslog.h>

using namespace std;

bool is_running = 1;

string configPath = "/home/stepa/daemon/backup_config.conf";

struct Config {
    std::string source_directory;
    std::string backup_directory;
    int backup_interval;
};

bool readConfig(const std::string& configFile, Config& config) {
    std::ifstream file(configFile);

    if (!file.is_open()) {
        syslog(LOG_ERR, "Error: Unable to open config file: %s", configFile.c_str());
        return false;
    }

    // Пример чтения конфигурационных данных
    file >> config.source_directory >> config.backup_directory >> config.backup_interval;

    file.close();

    return true;
}

void backupData(const std::string& sourceDir, const std::string& backupDir) {
    openlog("BackupDaemon", LOG_PID, LOG_DAEMON);
    std::string timestamp = std::to_string(std::time(0));
    std::string command = "cp -r " + sourceDir + " " + backupDir + "/backup_" + timestamp;
    system(command.c_str());
    
    
    syslog(LOG_INFO, "Backup completed successfully.");
    cout << "Backup compleated: " << timestamp << endl;
}

void daemonLoop(const std::string& configFile) {
    openlog("BackupDaemon", LOG_PID, LOG_DAEMON);

    while (true) {
    	if (is_running){
		Config config;

		if (!readConfig(configFile, config)) {
		    syslog(LOG_ERR, "Failed to read configuration file.");
		    return;
		}

		std::string sourceDir = config.source_directory;
		std::string backupDir = config.backup_directory;
		int backupInterval = config.backup_interval;

		// Выполнение резервного копирования
		backupData(sourceDir, backupDir);
		
		
		// Ожидание до следующей резервной копии
		std::this_thread::sleep_for(std::chrono::seconds(backupInterval));
	}	
    }
}

void pause_handler(int sig_num) {
    is_running = false;
    syslog(LOG_INFO, "Pause");
}

void continue_handler(int sig_num) {
    is_running = true;
    syslog(LOG_INFO, "Continue");
}

void my_terminate_handler(int sig_num) {
    syslog(LOG_INFO, "Terminate");
    closelog();
    exit(EXIT_SUCCESS);
}

void status_handler(int sig_num) {
    if (is_running) {
        syslog(LOG_INFO, "Daemon is running.");
    } else {
        syslog(LOG_INFO, "Daemon is paused.");
    }
}


void setup_signal_handlers() {
    signal(SIGTSTP, pause_handler);
    signal(SIGCONT, continue_handler);
    signal(SIGTERM, my_terminate_handler);
    signal(SIGUSR1, status_handler);
}

int main(int argc, char* argv[]) {
    openlog("BackupDaemon", LOG_PID, LOG_DAEMON);
    
    setup_signal_handlers();
    
    cout << "Starting daemon." << endl;
    syslog(LOG_INFO, "Starting daemon.");
    daemonLoop(configPath);

    closelog();
    return EXIT_SUCCESS;
}
