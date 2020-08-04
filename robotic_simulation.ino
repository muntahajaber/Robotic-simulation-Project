

//first copy

#include <PRIZM.h>     // include PRIZM library
  PRIZM prizm;  


String a,box1,box2,box3,box4;
String box1x,box1y,box2x,box2y,box3x,box3y,box4x,box4y;
String qrcode, data;


//*****

//turning  angle
float theta;
float turnAngle,destinationHeading;
//*****
float startingHeading = 0;
float currentHeading = startingHeading;
float distance = 0;

//*****
//current points
float xc,yc;
//target points
float xt ,yt;
//*****

/////////////////////////////////////////////////////////////////////////////////////////////////////// setUp

void setup(){
  
   prizm.PrizmBegin(); 
   prizm.setMotorInvert(1,1);

   //read from raspberrypi
   
  Serial.begin(9600);
          
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.println("qrcode");            //send to rassebperry pi QR
  while(!Serial.available()){}        //check if is it available or not
  a = Serial.readString();           //rasebperry pi send to arduino the data
  digitalWrite(13, HIGH);
  delay(2000);
     
  // a="(H,M,G,K)(Q,G,O,I)(T,L,R,N)(G,R,E,T)";

  box1=a.substring(1,8);
  box1x=substringa(box1,0,3);
  box1y=substringa(box1,4,box1.length());
  //*****
  //location robot (current position)
  xc=box1x.substring(0,1).toInt() ;
  yc=box1x.substring(2).toInt();
 //*****
  box2=a.substring(10,17);
  box2x=substringa(box2,0,3);
  box2y=substringa(box2,4,box2.length());
 //***** 
 //target position    (box2)
 xt=box2x.substring(0,1).toInt();
 yt=box2x.substring(2).toInt();
 //*****
  box3=a.substring(19,26);
  box3x= substringa(box3,0,3);
  box3y=substringa(box3,4,box3.length());
 //*****
  box4=a.substring(28,35);
  box4x=substringa(box4,0,3);
  box4y=substringa(box4,4,box4.length());


  //distance is 920mm--> 0.092m ?
                                                
  //must know the distince between (x,y)1 and (x,y)2 in parking zone to know after backword turn left or right? 
  
   prizm.setGreenLED(LOW);                // turn off green LED
   prizm.setRedLED(HIGH);                    //  turn red LED
   prizm.setMotorPowers(-35,-35);              //distance= 0.092m%2=0.046m vilocity=35,T=d*v=1.61
   delay(1510);                               //1.61s 
   prizm.setMotorPowers(125,125);            //stop
   delay(1000);                              //1s
  //*****
   prizm.setGreenLED(HIGH);               
   prizm.setRedLED(LOW); 
   delay(1000); 
   OutOfParkingZone();
  //***** 
  
  looptest(  box2x.substring(2).toInt(), box1x.substring(2).toInt(), box2x.substring(0,1).toInt(), box1x.substring(0,1).toInt());  
  
}

void looptest(int yt2, int yc2, int xt2, int xc2) {
   yt2= 76;
    yc2= 82;
 xt2= 84;
  xc2= 76;
  //*****
  //tan -1=atan 
  destinationHeading = atan2(yt2-yc2, xt2-xc2); //calculate turning angle
  destinationHeading = destinationHeading * 180/3.1415; //convert to degrees
  turnAngle = destinationHeading - currentHeading;
  
  //****
   
  if (turnAngle > 180) {
     turnAngle = turnAngle-360;
  }

  if (turnAngle < -180) {
    turnAngle = turnAngle+360;
  }

  //*****
  if (turnAngle < 0) {
    //prizm.setRedLED(HIGH);               
    prizm.setMotorPowers(50,-50);       // make a right turn
    delay(600);
  }

  if (turnAngle > 0) {
   // prizm.setRedLED(LOW);
    prizm.setMotorPowers(-50,50);       // make a left turn
    delay(600); 
  }
  if (turnAngle == 0){
   // prizm.setGreenLED(HIGH);
    //prizm.setMotorPowers(35,35);
    //delay(1200); 
  prizm.setRedLED(HIGH);
  delay(600);
  }

  
  //***** 
   distance = sqrt( ((xt2 - xc2)*(xt2 - xc2)) + ((yt2 - yc2)*(yt2 - yc2)) );
  if (distance >= 0) { 
    
    //avoid_Obstacle
     //if(prizm.readSonicSensorCM(4) > 4.5) {    // obstacle sense range set at 25 centimeters
       
        prizm.setMotorPowers(35,35);// go forward
        delay(distance*35);         //T=d*velocity;
       //}
      /* else{
       prizm.setMotorPowers(125,125);         // stop, obstacle detected
       delay(500);
       }*/
  
  }
  
  //***** 
  currentHeading = destinationHeading;
  //xc = xt;
  //yc = yt;
  //******
   
  prizm.PrizmEnd();
  


//avoid_Obstacle
/*
if(prizm.readSonicSensorCM(4) > 25)     // obstacle sense range set at 25 centimeters
  {
    prizm.setMotorPowers(35,35);          // forward while no obstacle detected 
    prizm.setRedLED(LOW);                 // turn off the red LED
    prizm.setGreenLED(HIGH);              // turn on green LED
  }
  else
  {//then it find wall or box-->[another box or target box] -->3 cases
    //for(int i=0;i<4;i++) <-- to reapet the action 4 time in smaller box

    
   prizm.setGreenLED(LOW);                // turn off green LED
   prizm.setRedLED(HIGH);                    // detected obstacle, turn red LED 
   prizm.setMotorPowers(125,125);         // stop, obstacle detected
   delay(500);
   prizm.setMotorPowers(-35,-35);         // back up
   delay(1000);
   prizm.setMotorPowers(125,125);         // stop
   delay(500);
   prizm.setMotorPowers(35,-35);          // make a right turn
   delay(500);
   
  }*/
  

  
                          
           

}



String substringa(String X,int i,int j){
  return X.substring(i,j);
}


void OutOfParkingZone(){
  
  
      int i=box1x.substring(0,1).toInt() ;
      int j=box1x.substring(2).toInt();      //knowing  (x,y)1 upper point of the inner parking zone
      int m=box1y.substring(0,1).toInt();
      int n=box1y.substring(2).toInt();     //knowing (x,y)2 lower point of the inner parking zone

                                 
                            //cases 1=up, 2=middle ,3=dwon   j=y1
                                int yaxis= Yaxis(j); 

                             //cases 1=left,2=middle,3= right     i=x1
                                int xaxis= Xaxis(i);  
                                
                             //[1,n][2,s][3,w,-->][4,e,<--]
                               int directionrobot=directionR(i,j,m,n);


                            positionRobot(yaxis,xaxis,directionrobot);
}

void positionRobot(int yaxis,int xaxis,int directionrobot){
//yaxis 1=up, 2=middle ,3=dwon  //xaxis 1=left,2=middle,3= right // directionrobot [1,n][2,s][3,w,-->][4,e,<--]

                             if((yaxis==1 &&  xaxis==1)||((yaxis==1 &&  xaxis==2))){       // UP Left || UP Middle
                                  
                                  switch(directionrobot){
                                    case 1: //left
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //invert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                            break;
                                    case 3: //left
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                    case 4://right
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(600);
                                            break;
                                  }
                                  
                             }else
                             if(yaxis==1 && xaxis==3){      //UP Right

                                  switch(directionrobot){
                                    case 1: //right
                                             prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(600); 
                                             break;
                                    case 2: //invert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                            break;
                                    case 3: //left
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                    case 4://right
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(600);
                                            break;
                                  }
                              
                             }else
                             if(yaxis==2 &&  xaxis==1){     //Middle Left

                                  switch(directionrobot){
                                    case 1: //left or innvert
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //right or invert
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                            delay(600);
                                            break;
                                    case 3: //invert or left or right
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             
                                            break;
                                    case 4://right or left
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(600);
                                            break;
                                  }
                              
                             }else
                             if(yaxis==2 &&  xaxis==2){   //middle midlle

                                  switch(directionrobot){
                                    case 1: //left or right or invert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //left or right or invert
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(1000);
                                            
                                            break;
                                    case 3: //left or right or invert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             
                                            break;
                                    case 4://left or right or invert
                                            break;
                                  }
                              
                             }else
                             if(yaxis==2 &&  xaxis==3){   //middle left

                                switch(directionrobot){
                                    case 1: //right or innvert
                                             prizm.setMotorPowers(50,-50);       // make a right turn
                                             delay(600); 
                                             break;
                                    case 2: //left or invert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                    case 3: //invert or left or right
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                            break;
                                    case 4://invert or left or right
                                           prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                            break;
                                  }
                              
                             }else
                             if(yaxis==3 &&  xaxis==1){     //down Left

                                switch(directionrobot){
                                    case 1: //left or innvert
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //right or invert
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                            delay(600);
                                            break;
                                    case 3: //invert or right
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                            break;
                                    case 4://left
                                           prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                  }
                              
                             }else
                             if(yaxis==3 &&  xaxis==2){   //down middle

                                switch(directionrobot){
                                    case 1: //left or innvert or right
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //right or left
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                            delay(600);                         // wait here for .6 seconds while motors spin
                                            break;
                                    case 3: // right or invert
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                            delay(600);
                                            break;
                                    case 4://left or invert
                                           prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                  }
                              
                             }else
                             if(yaxis==3 &&  xaxis==3){   //down right

                                switch(directionrobot){
                                    case 1: //innvert or right
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             prizm.setMotorPowers(-50,50);       // make a left turn
                                             delay(600); 
                                             break;
                                    case 2: //left
                                            prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                    case 3: // right 
                                            prizm.setMotorPowers(50,-50);       // make a right turn
                                            delay(600);
                                            break;
                                    case 4://left or invert
                                           prizm.setMotorPowers(-50,50);       // make a left turn
                                            delay(600);
                                            break;
                                  }
                                    
                             }
                                             prizm.setMotorPowers(125,125);      // stop both motors with in brake mode
                                             delay(1000); 

}

int Yaxis(int y){    //cases 1=up, 2=middle ,3=dwon
        
 
        if(y<72){     //up (A-G)

               return 1;
        }else 
          if(y<79){       //middle (H-N)
          
               return 2;
          }else{             //down (O-U)
            
               return 3;
            }

}

int Xaxis(int x){    //cases 1=left,2=middle,3= right 


  
    if(x<72){  ///in left corner  (A-G)

                  return 1;
           }
          else if(x<79){ //in middle (H-N)

                 return 2;
                  
                }
          else {              //in right corner (O-U)
                  return 3;

                  }

}

int directionR(int x1,int y1,int x2,int y2){                   ///[1,n][2,s][3,w,<—][4,e,—>]

  int x=x1-x2;
  int y=y1-y2;
  
  
  if(y<0){    //case 1
    return 1;
  }
  else
  if(y==0){//case 3,4
    if(x>0)
    return 3;
    else
    return 4;
  }
  else
  if(y>0){//case 2 
    return 2;
    
  }

  return 0;
  
}
