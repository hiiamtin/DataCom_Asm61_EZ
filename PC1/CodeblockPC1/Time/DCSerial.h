/*
    # Protocol Design
i   |   7   |   6   5   4          |   3   2   |   1   0   |
t   |   0   |   CONTROL            |   ANGLE   |   SIZE    |

        0       010 : SEVO         00 : 45         00 : NULL
        0       011 : ACK_PC1      01 : 90         01 : SMALL
        0       100 : 3PIC         10 : 135        10 : MEDIUM
        0       101 : 1PIC         11 : NULL       11 : LARGE
        0       110 : ACK_SERVO
*/

#define BAUD_RATE 9600

#define CONTROL_SERVO       2   /* from PC2 -> A2 */
#define CONTROL_PC1ACK      3   /* from PC2 -> A2 */

#define CONTROL_TREEPIC     4   /* from A2 -> PC2 */
#define CONTROL_ONEPIC      5   /* from A2 -> PC2 */
#define CONTROL_SERVOACK    6   /* from A2 -> PC2 */

#define SERVO_ACK 0x6c

char getControl(char data) {
    return data>>4;
}

char getAngle(char data) {
    return (data>>2) & 0x03;
}

char getSize(char data) {
    return data & 0x03;
}

char setAckPc1(char data) {
    return data | 0x30;
}
