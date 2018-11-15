#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <stdlib.h>
#include "Serial.h"
#include <iostream>
#include <cmath>
#include <string>
#include <string.h>
#include "DCImg.h"
#include "DCSerial.h"
#define COM_PORT_A "\\\\.\\COM16"
#define TIME_OUT 40
#define FRAME_LENGTH 19
#define TEST_SINGLE false

using namespace std;
char incomingData[256] = "";
unsigned int dataLength = FRAME_LENGTH;
int readResult = 0;
Serial *SPS;
bool status = true;
bool suc = false;
int angle[3]={0};
int temp[3]={0};
int sizes=0;
void sendACK();
void initialcam();
void calipixel();
FILE* fp;
int c,d=0,g=0,value[3]= {0,0,0},i,z,j,ii,r[3]= {0,1,2};
FILE *fptr[3];
int inSize = 4;
int pic_id = 0;
int state = 0;
char input;

int main()
{
	string frameType, frameData;
	int found, tmpi;



	cout << "Connecting to " << COM_PORT_A << ".." << endl;
	SPS = new Serial(COM_PORT_A);

	if (SPS->IsConnected())
	{
		cout << "We're connected!" << endl << endl;
		while(status){
        cout << "Press C to start" << endl;
        cin >> input;

            if(input == 'C'){
                cout << "Connecting" << endl;
                string data = "174762";
                SPS->WriteData(data.c_str(),data.length());
                cout << "Waiting for ACK" << endl;
                do{
                   SPS->ReadData(incomingData,dataLength);
                }while(strcmp(incomingData,"873813") != 0);
                cout << "Recieved ACK" << endl;
                 cout << "connected" << endl;
                cout << "Press S for Small circle" << endl;
                cout << "Press M for Medium circle" << endl;
                cout << "Press L for Large circle" << endl;
                cout << "Press C to stop" << endl << endl;
                status = false;
            }

		}
	}
	while(SPS->IsConnected()){
            if(state == 0){
                cin >> input;
                if(input == 'S'){
                    SPS->WriteData("1048577",7);
                    cout << "Request for Small circle" << endl;
                    Sleep(6000);
                     cout << "--- Data Sented ---" << endl;
                }
                else if(input == 'M'){
                    SPS->WriteData("1048578",7);
                    cout << "Request for Medium circle" << endl;
                    Sleep(6000);
                     cout << "--- Data Sented ---" << endl;
                }
                else if(input == 'L'){
                    SPS->WriteData("1048579",7);
                    cout << "Request for Large circle" << endl;
                    Sleep(6000);
                     cout << "--- Data Sented ---" << endl;
                }
                else if(input == 'C'){
                    SPS->WriteData("174762",6);
                    Sleep(6000);
                     cout << "--- Data Sented ---" << endl;
                      cout << "--- Disconnected ---" << endl;
                      return 0;
                }
                state = 2;
            }
            else{
      readResult = 0;
      readResult = SPS->ReadData(incomingData,dataLength);
      if (readResult != 0)
		{
		    if(strcmp(incomingData,"873813") == 0){
                    cout << "Recieved ACK" << endl;
		    }
		    else{
                if(state == 2){
                    cout << "Black Pixel : ";
                    int pixel = atoi(incomingData);
                    pixel &= 262143;
                    cout << pixel << endl;
                }
                else if(state == 1){
                    cout << "Angle : ";
                    int anglein = atoi(incomingData);
                    anglein &= 3;
                    if(anglein == 0){
                        cout << "0 Degree" << endl;
                    }
                    else if(anglein == 1){
                        cout << "45 Degree" << endl;
                    }
                    else if(anglein == 2){
                        cout << "90 Degree" << endl;
                    }
                }
                state --;
                if(state == 0){
                        Sleep(3000);
                    sendACK();
                }
		    }
            }
            }
		}

	}


void sendACK()
{
	string outgoingData;
        outgoingData = "873813";
	cout << "--- Sending ACK ---" << endl;
	SPS->WriteData(outgoingData.c_str(), outgoingData.length());
	Sleep(6000);
	cout << "--- Sended ACK ---" << endl;
	Sleep(6000);
}



void initialcam(){
for(int i=0; i<3; i++)
    {

        fp = waitNextBMP(fp, pic_id);
        //char output[21];
            if(i==0)
            {
                SPS->WriteData("1", 1);
                Sleep(400);
            }
            else if(i==1)
            {
                SPS->WriteData("2", 1);
                Sleep(400);
            }
            else if(i==2)
            {
                SPS->WriteData("3", 1);
                Sleep(400);
            }



        pic_id++;
    }
    }

void calipixel(){
    fptr[0]=fopen("C:\\out\\0.bmp","rb");
    fptr[1]=fopen("C:\\out\\1.bmp","rb");
    fptr[2]=fopen("C:\\out\\2.bmp","rb");
    for(i=0; i<3; i++)
    {
        do
        {
            c=fgetc(fptr[i]);

            d++;
            if(d > 54)
            {
                if(g == 1)
                {
                    if(c < 90 )
                    {
                        value[i]++;
                    }

                }
                g++;
                if(g== 3)
                    g=0;
            }
        }
        while(c!=EOF);
        fclose(fptr[i]);
        printf("pic%d size : %d\n",i+1,value[i]);
    }
        for(i=0;i<3;i++){
        for(j=0;j<2;j++){
            if(value[j]>value[j+1]){
                z=r[j];
                r[j]=r[j+1];
                r[j+1]=z;
                z=value[j];
                value[j]=value[j+1];
                value[j+1]=z;
            }
        }
    }
    printf("sort of index : %d %d %d\n",r[0],r[1],r[2]);
 // fprintf(txt, "unsigned long value[3] =  ");
 // fprintf(txt, "{%d,%d,%d};",value[0],value[1],value[2]);
 // fprintf(txt, "\nunsigned long r[3] =  ");
//  fprintf(txt, "{%d,%d,%d};",r[0],r[1],r[2]);
 // fclose(txt);
}
