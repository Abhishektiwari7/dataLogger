//https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
//https://www.equestionanswers.com/c/file-append-mode.php
//fprintf https://www.tutorialspoint.com/c_standard_library/c_function_fprintf.htm //time
//https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

#include <sys/stat.h>
#include <time.h>           //for timelog timestamp
#define SERIALTERMINAL      "/dev/ttyUSB5" //my usb port check yours?
#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

FILE *file;	//file to open object

int set_interface_attribs(int fd, int speed) {
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    tty.c_lflag |= ICANON | ISIG;  /* canonical input */
    tty.c_lflag &= ~(ECHO | ECHOE | ECHONL | IEXTEN);

    tty.c_iflag &= ~IGNCR;  /* preserve carriage return */
    tty.c_iflag &= ~INPCK;
    tty.c_iflag &= ~(INLCR | ICRNL | IUCLC | IMAXBEL);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);   /* no SW flowcontrol */

    tty.c_oflag &= ~OPOST;

    tty.c_cc[VEOL] = 0;
    tty.c_cc[VEOL2] = 0;
    tty.c_cc[VEOF] = 0x04;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


int main() {
    char *portname = SERIALTERMINAL;
    int fd;
    int wlen;

    //------for--time stamp---
    time_t rawtime; 
    struct tm *timeinfo; 
    //---------------------

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B115200);

    /* simple output */
    // wlen = write(fd, "Hello!\n", 7);
    // if (wlen != 7) {
    //     printf("Error from write: %d, %d\n", wlen, errno);
    // }
    // tcdrain(fd);    /* delay for output */


    /* simple canonical input */
    do {
        unsigned char buf[124] = {0};
        int rdlen;
        rdlen = read(fd, buf, sizeof(buf) - 1); //read from serial
        file = fopen( "DATA.txt", "a+" ); //apppend and read //file = fopen( "DATA.txt", "w+" ); read and write only
        if (rdlen > 0) { //if something there then write in file
            buf[rdlen] = '\0';
            
            time ( &rawtime );                    //for local time stamp
            timeinfo = localtime ( &rawtime );
            fprintf(file,"%s%s",asctime (timeinfo),buf); /*fprintf(file,"%s",asctime (timeinfo)); 
                                                            fprintf(file,"%s",buf);
                                                         */
            //fwrite (&buf, sizeof(buf), 1, file); //future use
            printf("\nSerial Data:%s\n",buf);
        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        } else {  /* rdlen == 0 */
            printf("Nothing read. EOF?\n");
        }
        fclose (file); //close the file when  done from write               
        /* repeat read */
    } while (1);
}