#include <stdio.h>
#include <tchar.h>
#include "Serial.h"
#include <iostream>
#include <cmath>
#include <string>
#include <string.h>
#include "DCImg.h"
#include "DCSerial.h"
#define COM_PORT_A "\\\\.\\COM9"
#define TIME_OUT 2
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

int main()
{
    //txt=fopen("C:\\Users\\Oakk\\Desktop\\txRx\\text.h","w");
	string frameType, frameData;
	int found, tmpi;



	cout << "Connecting to " << COM_PORT_A << ".." << endl;
	SPS = new Serial(COM_PORT_A);

	if (SPS->IsConnected())
	{
		cout << "We're connected!" << endl << endl;
		cout << "Calibating" << endl << endl;
        initialcam();
        calipixel();
        cout << endl << "Calibating Done" << endl;
        cout << endl << "Waiting for Connection" << endl;
		while(status){
		readResult = 0;
		readResult = SPS->ReadData(incomingData,dataLength);
		Sleep(100);
		if (readResult != 0)
		{
		    cout << incomingData << endl ;

            if(strcmp(incomingData,"174762") == 0){
                cout << "Connecting" << endl;
                sendACK();
                cout << "Connected" << endl;
                status = false;
            }
		}
		}
	}
	while(SPS->IsConnected()){
      readResult = 0;
      readResult = SPS->ReadData(incomingData,dataLength);
      if (readResult != 0)
		{
		    if(strcmp(incomingData,"1048577") == 0){
                cout << "Get requested for Small Circle" << endl;
                sendACK();
                if(r[0] == 0){
                SPS->WriteData("3", 1);
                }
                else if(r[0] == 1){
                SPS->WriteData("1", 1);
                }
                else if(r[0] == 2){
                SPS->WriteData("2", 1);
                }
                long dataout = 1 << 18;
                dataout += value[0];
                char outgoingdata[19];
                sprintf (outgoingdata, "%lu" , dataout );
                SPS->WriteData(outgoingdata,19);
                Sleep(6000);
                cout << "--- Pixel Sented ---" << endl;
                dataout = 2<<18;
                dataout += r[0];
                char camangle[19];
                sprintf (camangle, "%lu" , dataout );
                SPS->WriteData(camangle,19);
                Sleep(6000);
                cout << "--- Angle sented ---" << endl;
		    }
		    else if(strcmp(incomingData,"1048578") == 0){
		        cout << "Get requested for Medium Circle" << endl;
		       sendACK();
                if(r[1] == 0){
                SPS->WriteData("3", 1);
                }
                else if(r[1] == 1){
                SPS->WriteData("1", 1);
                }
                else if(r[1] == 2){
                SPS->WriteData("2", 1);
                }
                long dataout = 1 << 18;
                dataout += value[1];
                char outgoingdata[19];
                sprintf (outgoingdata, "%lu" , dataout );
                SPS->WriteData(outgoingdata,19);
                Sleep(6000);
                cout << "--- Pixel Sented ---" << endl;
                dataout = 2<<18;
                dataout += r[1];
                char camangle[19];
                sprintf (camangle, "%lu" , dataout );
                SPS->WriteData(camangle,19);
                Sleep(6000);
                cout << "--- Angle sented ---" << endl;
		    }
		    else if(strcmp(incomingData,"1048579") == 0){
		        cout << "Get requested for Large Circle" << endl;
		       sendACK();
                if(r[2] == 0){
                SPS->WriteData("3", 1);
                }
                else if(r[2] == 1){
                SPS->WriteData("1", 1);
                }
                else if(r[2] == 2){
                SPS->WriteData("2", 1);
                }
                long dataout = 1 << 18;
                dataout += value[2];
                char outgoingdata[19];
                sprintf (outgoingdata, "%lu" , dataout );
                SPS->WriteData(outgoingdata,19);
                Sleep(6000);
                cout << "--- Pixel Sented ---" << endl;
                dataout = 2<<18;
                dataout += r[2];
                char camangle[19];
                sprintf (camangle, "%lu" , dataout );
                SPS->WriteData(camangle,19);
                Sleep(6000);
                cout << "--- Angle sented ---" << endl;
		    }
		    else if(strcmp(incomingData,"174762") == 0){
                    sendACK();
                    cout << "Disconnected" << endl;
                    return 0;
               //Terminate
		    }
		    else if(strcmp(incomingData,"873813")){
                cout << "--- Received ACK ---" << endl;
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
}
