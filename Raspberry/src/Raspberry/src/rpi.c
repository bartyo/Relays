#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/***PROTOTYPES***/
void setup();
void loop();

void ask_sensor();
void analyse_sensor_response();

void add_stack(char sensor);
void remove_stack(char pro);
void read_http(char char_serial_read);
void read_ble(char char_serial_read);
void send_http();
void send_ble();
void process_data();
void save_data();
void check_alive();
void delete_device();
void pair_device();
char existing_sensor();
void reset_buffer(char* buffer);
void display(char* msg);

int main()
{
    setup();
    while(0)
    {
        loop();
    }
}

//Global variable begin with g_
//Constant are uppercase
//Structure begin with struct_

/***BUFFER***/
#define NB_BUFFER_HTTP 200
#define BLE_DATA_LONG 9
#define NB_STACK_BLE 30
#define NUMBER_DATA_SAVED 30
char g_buffer_http[NB_BUFFER_HTTP] = {0};//ID = 2 bytes
char g_buffer_ble[BLE_DATA_LONG] = {0};//buffer de reception ble
short g_ble_stack_id[NB_STACK_BLE] = {0};//Stack BLE

/***DEVICES***/
typedef struct{
  short ID;
  char O2;
  char GainO2;
  char pulse;
  char GainPulse;
  char _status;//b0:working;b1:enable
  char dataSaved;//To know what is the index of data in data[][index]
  char data[BLE_DATA_LONG][NUMBER_DATA_SAVED];//Save of the 30 previous data
}struct_sensor;

char* trame_ble = "azertyuoip";

#define NB_SENSORS 100
struct_sensor g_sensor[NB_SENSORS];

char g_sensor_processing = 0;//permet à toutes les fonctions de connaitre le sensor en cours

/***TIMER***/
clock_t timer = 0;
#define HTTP_TIME 100000
#define BLE_TIME 100000
#define CHECK_ALIVE_TIME 100000

/***HTTP***/
#define BEGIN_HTTP '<'
#define END_HTTP '>'
char g_token_send_HTTP = 0;
char g_begin_reception_HTTP = 0;
char g_end_reception_HTTP = 0;

/***BLE***/
#define BEGIN_BLE '('
#define END_BLE ')'
char g_token_send_BLE = 0;
char g_begin_reception_BLE = 0;
char g_end_reception_BLE = 0;

void setup() {
  //Configuration HTTP/WIFI
  //Configuration Bluetooth
  //Initialisation watchdog
  //initialisation timer
  //system("curl");
  timer = clock();
}



char g_char_loop_http = 0;
char g_char_loop_ble = 0;

void loop() {
  char char_serial_read = 0;
  //Watchdog
  //Lecture http
    //Si message lu en entier => analyse_message_http
      //Détecter début de message
        //stocker le message
      //Détecter la fin du message
        //Analyser le message en entier (format JSON)
    if(g_token_send_HTTP == 0 && g_token_send_BLE == 0 && kbhit())
    {
        char_serial_read = getch();
        printf("%c", char_serial_read);
        //Serial.print(char_serial_read);
    }
    read_http(char_serial_read);

  
  //Lecture bluetooth
    //Si message lu en entier => analyse_message_bluetooth
      ////Détecter début de message
        //Tous les 8 bits, on analyse le mot et on le stock
      //Détecter la fin du message
        //Tester le crc
          //Si crc faux on efface les données
          //Sinon analyse du message
            //Si le message correspond à un requete de la stack => ok
            //Sinon on lève une erreur
  read_ble(char_serial_read);
  

  //Tous les 1/30s envoi requete http
    //Construire le message avec les données des sensors "sensors"
  if(clock()-timer > HTTP_TIME)
  {
    //Sent of all data of all sensor
    send_http();
  }

  //Tous les 1/30s envoi requete bluetooth (message prédéfini)
    //Stocker de la requete dans stack_bluetooth
  if(clock()-timer > BLE_TIME)
  {
    send_ble();
  }

  if(clock()-timer > CHECK_ALIVE_TIME)
  {
    check_alive();
  }
  
}

/*
#define BEGIN_RECEPTION_BLE 1
#define END_RECEPTION_BLE 2
#define BEGIN_RECEPTION_HTTP 4
#define END_RECEPTION_HTTP 8
#define TOKEN_ENVOI_BLE 16
#define TOKEN_ENVOI_HTTP 32
char g_receive_status = 0;
/***HTTP***
#define BEGIN_HTTP '<'
#define END_HTTP '>'
bool g_token_send_HTTP = false;

/***BLE***
#define BEGIN_BLE '<'
#define END_BLE '>'
bool g_token_send_BLE = false;
*/

/*****************************************************************************/
void read_http(char char_serial_read)//
{
    if(char_serial_read == '/'){display(g_buffer_http);}
    if(char_serial_read == BEGIN_HTTP)
    {
        reset_buffer(g_buffer_http);
        g_begin_reception_HTTP = 1;
        g_end_reception_HTTP = 0;
    }
    if(char_serial_read == END_HTTP)
    {
        g_begin_reception_HTTP = 0;
        g_end_reception_HTTP = 1;
    }
    if(char_serial_read != BEGIN_HTTP && char_serial_read != END_HTTP && g_begin_reception_HTTP == 1 && g_end_reception_HTTP == 0 && char_serial_read != 0)
    {
        //printf("coucou");
        g_buffer_http[g_char_loop_http] = char_serial_read;
        g_char_loop_http++;
    }
    if(g_begin_reception_HTTP == 0 && g_end_reception_HTTP == 1)//Receipt finished
    {
        g_char_loop_http = 0;
    }
}

/*****************************************************************************/
void read_ble(char char_serial_read)
{
  if(char_serial_read == '&'){display(g_buffer_ble);}
  if(char_serial_read == BEGIN_BLE)
  {
    reset_buffer(g_buffer_ble);
    g_begin_reception_BLE = 1;
    g_end_reception_BLE = 0;
  }
  if(char_serial_read == END_BLE)
  {
    g_begin_reception_BLE = 0;
    g_end_reception_BLE = 1;
  }
  if(char_serial_read != BEGIN_BLE && char_serial_read != END_BLE && g_begin_reception_BLE == 1 && g_end_reception_BLE == 0 && char_serial_read != 0)
  {
    
    g_buffer_ble[g_char_loop_ble] = char_serial_read;
    g_char_loop_ble++;
  }
  if(g_begin_reception_BLE == 0 && g_end_reception_BLE == 1)//Receipt finished
  {
    g_char_loop_ble = 0;
  }
  
  
  //Find sensor processing
  if(0==1)
  {
    if(existing_sensor() > 0)
    {

    }
  }
  //If g_buffer_ble received entire data
  for(char j=0;j<NB_STACK_BLE;j++)
  {
    if(g_ble_stack_id[j] & 0xF0 == g_buffer_ble[0] && g_ble_stack_id[j] & 0x0F == g_buffer_ble[1])/*if ble id read is into stack then remove it from stack*/
    {
      //remove_stack(j);
    }
  }
  //if message is finished
  //save_data();

  //If we got 30 data
  //process_data();
}

/*****************************************************************************/
void send_http()
{
  //send message http
  char i = 0;
  for(i=0;i<NB_SENSORS;i++)
  {
    if(g_sensor[i]._status & 0x01)
    {
      //envoi du message JSON => library to convert struct into json
    }
  }
}

/*****************************************************************************/
void send_ble()
{
  char i = 0;
  for(i=0;i<NB_SENSORS;i++)
  {
    if(g_sensor[i]._status & 0x01)
    {
      //envoi du message g_sensors[i].ID
      add_stack(i); 
    }
  }
}

/*****************************************************************************/
void add_stack(char sensor)
{
  //Add sensor ID to g_ble_stack_id
  char i = 0;
  char b = 0;
  for(i=0;i<NB_STACK_BLE;i++)
  {
    if(g_ble_stack_id[i] == g_sensor[sensor].ID)
    {//Sensor already exists into stack ble
      
    }
    else
    {
      b++;
    }
  }
  if(b>0)
  {
    g_ble_stack_id[b] = g_sensor[sensor].ID;
 
  }
}

/*****************************************************************************/
void remove_stack(char pro)
{
  g_ble_stack_id[pro] = 0;
}

/*****************************************************************************/
void process_data()
{
  
}

/*****************************************************************************/
void save_data()
{ //Save data just read with ble
  //char g_buffer_ble[BLE_DATA_LONG] contains data just read.
  if(existing_sensor() > 0)
  {
      //Save data received for calculation
      for(char j=0;j<BLE_DATA_LONG;j++)
      {
        g_sensor[g_sensor_processing].data[j][g_sensor[g_sensor_processing].dataSaved];
      }
      
      //Update NUMBER_DATA_SAVED
      if(g_sensor[g_sensor_processing].dataSaved < NUMBER_DATA_SAVED)
      {
        g_sensor[g_sensor_processing].dataSaved++;
      }
      else
      {
        g_sensor[g_sensor_processing].dataSaved = 0;
      }
  }
}

/*****************************************************************************/
void check_alive()
{//Useless, because this check will be done before each http sending
  for(char i=0;i<NB_STACK_BLE;i++)
  {
    for(char j=0;j<NB_SENSORS;j++)
    {
      if(g_ble_stack_id[i]==g_sensor[j].ID)
      {
        //Device is the stack, so it is dead
        g_sensor[j]._status |= 0x01;
      }
    }

  }
}

/*****************************************************************************/
void delete_device()
{
  //Function called by http_analyse function to delete device which are not used.
  g_sensor[g_sensor_processing].ID = 0;
  g_sensor[g_sensor_processing].O2 = 0;
  g_sensor[g_sensor_processing].GainO2 = 0;
  g_sensor[g_sensor_processing].pulse = 0;
  g_sensor[g_sensor_processing].GainPulse = 0;
  for(char i=0;i<BLE_DATA_LONG;i++)
  {
    for(char j=0;i<NUMBER_DATA_SAVED;i++)
    {
      g_sensor[g_sensor_processing].data[i][j] = 0;
    }
  }
  
}

/*****************************************************************************/
void pair_device()
{
  //Add a new device if it isn't into g_sensor trame_ble
  short buffer_response = ((0x0000 | g_buffer_ble[0]) << 8) | g_buffer_ble[1];
  if(existing_sensor() < 0)
  {
    //Pair new sensor
  }
}

/*****************************************************************************/
char existing_sensor()
{
  short buffer_response = ((0x0000 | g_buffer_ble[0]) << 8) | g_buffer_ble[1];
  char existing_sensor = -1;
  for(char i=0;i<NB_SENSORS;i++)
  {
    if(g_sensor[i].ID == buffer_response)
    {
      existing_sensor = 1;
      g_sensor_processing = i;
    }
  }
  return existing_sensor;
}

/*****************************************************************************/
void reset_buffer(char* buffer)
{
  char i=0;
  while(buffer[i] != 0)
  {
    buffer[i] = 0;
    i++;
  }
}

/*****************************************************************************/
void display(char* msg)
{
  char i=0;
  while(msg[i] != 0)
  {
    //Serial.print(msg[i]);
    printf("%c", msg);
    i++;
  }
  printf("\n");
  //Serial.println();
}