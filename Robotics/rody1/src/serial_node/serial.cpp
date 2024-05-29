#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <csignal>
#include <chrono>
#include <iostream>
#include <libusbp.hpp>
#include <memory>
#include <streambuf>
#include <thread>

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>  // Include this for geometry_msgs::Twist
#include "rody1_messages/Controller.h"
// #include <romusa_messages/odom.h>
// #include <romusa_messages/cmd_vel.h>
// #include <romusa_messages/cmd_slider.h>
// #include <romusa_messages/vac.h>
// #include <romusa_messages/enc_slider.h>

// Linux headers
#include <errno.h>   // Error integer and strerror() function
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()


#define STM_VID 0x0483
#define STM_PID 0x5740
// #define STM_VID 0x0403  //0403:6001
// #define STM_PID 0x6001

uint8_t stm_to_pc[43];
uint8_t pc_to_stm[43] = {'i', 't', 's'};

uint8_t stm_to_pc_copy[43];
uint8_t pc_to_stm_copy[43] = {'i', 't', 's'};

int serial_port;
int invalidDataCount = 0;
uint32_t pub_bandwidth = 0;
volatile std::sig_atomic_t flag = false;
ros::Time start_time;

ros::Subscriber sub_controller;

void reset_usb_stm();

void signalHandler(int signal) {
    flag = true;
}
void communicator_callback(const ros::TimerEvent &event);
void transfer_monitor_callback(const ros::TimerEvent &event);
void publisher_callback(const ros::TimerEvent &event);
void controllerCallback(const rody1_messages::Controller::ConstPtr &Controller_msg);

int initialize_tty(const char *dev_path);
std::string discover_STM(uint16_t vid, uint16_t pid);


int main(int argc, char **argv)
{
  std::signal(SIGINT, signalHandler);
  ros::init(argc, argv, "serial_node");
  printf("Serial node started\n");
  ros::NodeHandle nh;
  start_time = ros::Time::now();

  std::string dev_path = discover_STM(STM_VID, STM_PID);
  if (dev_path != "NULL")
  {
    // serial_port = initialize_tty(dev_path.c_str());
    // serial_port = initialize_tty("/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0");
    // serial_port = initialize_tty("/dev/serial/by-id/usb-STMicroelectronics_STM32_Virtual_ComPort_20913786424D-if00");
    serial_port = initialize_tty(dev_path.c_str());
    printf("Master discovered at %s\n", dev_path.c_str());
  }
  else
  {
    while (dev_path == "NULL" && !flag)
    {
      printf("Master not found, retry!!!\n");
      dev_path = discover_STM(STM_VID, STM_PID);
      if(flag){
        printf("Interrupted by user. Exiting...\n");
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));

    } 
    // serial_port = initialize_tty("/dev/serial/by-id/usb-STMicroelectronics_STM32_Virtual_ComPort_20913786424D-if00");
    serial_port = initialize_tty(dev_path.c_str());

    printf("Master discovered at %s\n", dev_path.c_str());
  }
  printf("sizeof stm_to_pc: %ld\n", sizeof(stm_to_pc));
  printf("sizeof pc_to_stm: %ld\n", sizeof(pc_to_stm));

  reset_usb_stm();

  //start timers
  ros::Timer timer = nh.createTimer(ros::Duration(0.001), communicator_callback);
  ros::Timer timer2 = nh.createTimer(ros::Duration(1), transfer_monitor_callback);
  ros::Timer timer3 = nh.createTimer(ros::Duration(0.002), publisher_callback);
  
  // sub_vel = nh.subscribe("cmd_vel", 100, velCallback); // Subscribe to cmd_vel topic
  // sub_vel = nh.subscribe("cmd_vel", 100, velCallback); // Subscribe to cmd_vel topic
  sub_controller = nh.subscribe("Controller", 10, controllerCallback); // Subscribe to Controller topic

  ros::MultiThreadedSpinner spinner(4);
  spinner.spin();
  return 0;
}

void communicator_callback(const ros::TimerEvent &event) {
  // Get the current time
  ros::Time current_time = ros::Time::now();

  uint8_t buffer[43];

  // Fetch to_stm data and copy to pc_to_stm to be send to STM32

  //check sum to send data to stm
  uint16_t check_sum = 0;
  for (int i = 3; i < 41; i++)
  {
    check_sum += pc_to_stm[i];
  }
  check_sum = ~check_sum;

  memcpy(pc_to_stm + 41, &check_sum, 2);

  write(serial_port, pc_to_stm, 43);

  // Read data from STM32 after sending, and store it to stm_to_pc
  int num_bytes = read(serial_port, buffer, 43);
  // printf("num_bytes: %d\n", num_bytes);
  if (num_bytes == 43) {
    invalidDataCount = 0;
    memcpy(stm_to_pc, buffer, 43);
    // memcpy(stm_to_pc_copy, stm_to_pc, 43);
  } else {
    invalidDataCount++;
    printf("Data Invalid count : %d\r\n", invalidDataCount);
    if (invalidDataCount < 10)
      return;
    std::string dev_path = discover_STM(STM_VID, STM_PID);
    // TODO : Add exception handler for when the device is unplugged or not responding
    if (dev_path == "NULL")  // device is not found
      std::cerr << "Can't find STM\r\n";
    else
      serial_port = initialize_tty(dev_path.c_str());
  }
}

void transfer_monitor_callback(const ros::TimerEvent &event) {
  // Get the current time
  ros::Time current_time = ros::Time::now();
  // Calculate the time elapsed in milliseconds
  double elapsed_time_ms = (current_time - start_time).toSec() * 1000;
  printf("%f Publisher with Bandwidth %dHz\r\n", elapsed_time_ms, pub_bandwidth);
  pub_bandwidth = 0;
}

void publisher_callback(const ros::TimerEvent &event) {
  // Get the current time
  ros::Time current_time = ros::Time::now();

  // Calculate the time elapsed in milliseconds
  double elapsed_time_ms = (current_time - start_time).toSec() * 1000;

  pub_bandwidth++;
  
}

void controllerCallback(const rody1_messages::Controller::ConstPtr &Controller_msg)
{
  memcpy(pc_to_stm + 3, &Controller_msg->velx, 4);
  memcpy(pc_to_stm + 7, &Controller_msg->vely, 4);
  memcpy(pc_to_stm + 11, &Controller_msg->angvel, 4);
}

std::string discover_STM(uint16_t vid, uint16_t pid)
{
  std::string dev_path;
  libusbp::device device = libusbp::find_device_with_vid_pid(vid, pid);
  if (!device)
  {
    return "NULL";
  }
  else
  {
    for (int i = 0; i < 2; i++)
    {
      try
      {
        libusbp::serial_port port(device, i, true);
        std::string port_name = port.get_name();
        dev_path = port_name;
        return dev_path;
      }
      catch (const std::exception &error)
      {
        std::cerr << "Exception " << error.what() << std::endl;
        return "NULL";
      }
    }
  }
  // we should've not arrived here....
  return "NULL";
}

int initialize_tty(const char *dev_path)
{
  int serial_port = open(dev_path, O_RDWR);
  printf("serial_port: %d\n", serial_port);
  if (serial_port < 0)  
  {
    std::cerr << "Error " << errno << " from open:" << strerror(errno) << std::endl;
    return -1;
  }
  // Create new termios struct, we call it 'tty' for convention
  // No need for "= {0}" at the end as we'll immediately  the existing
  // config to this struct
  struct termios tty;

  //  in existing settings, and handle any error
  // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
  // must have been initialized with a call to tcgetattr() overwise behaviour
  // is undefined
  if (tcgetattr(serial_port, &tty) != 0)
  {
    std::cerr << "Error " << errno << " from tcgetattr:" << strerror(errno) << std::endl;
    return -1;
  }

  tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE;         // Clear all bits that set the data size
  tty.c_cflag |= CS8;            // 8 bits per byte (most common)
  tty.c_cflag |= CRTSCTS;        // Enable RTS/CTS hardware flow control
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;                                                        // Disable echo
  tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
  tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
  tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
  {
    std::cerr << "Error " << errno << " from tcsetattr:" << strerror(errno) << std::endl;
    return -1;
  }
  printf("Opened %s serial port with fd %d\n", dev_path, serial_port);
  return serial_port;
}

void reset_usb_stm()
{
  // float initial_speed[3] = {0, 0, 0};
  // short int initial_arm[2] = {0, 100};
  // short int initial_lifter[2] = {0, 100};
  // bool initial_vac_pneu[2] = {0, 0};
  // memcpy(pc_to_stm + 3, initial_speed, 12);
  // memcpy(pc_to_stm + 15, initial_arm, 4);
  // memcpy(pc_to_stm + 19, initial_lifter, 4);
  // memcpy(pc_to_stm + 23, initial_vac_pneu, 2);
}