//DICHIARAZIONI VARIABILI
//Facciamo prima ad hackerare la NASA.

float myState[12];
float otherState[12];
float myPos[3];
float otherPos[3];
float debris[41][4];                //Matrice dei detriti
float debris16[2];                  //Array del debris in basso a destra
float debris32[2];
float puntoPP[2];                   //Coordinata ++
float distanceGreenPP;              //Distanza verde ++
float distancePurplePP;             //Distanza viola ++
float debris13[2];                  //Array del debris in basso a sinistra
float debris29[2];
float puntoNP[2];                   //Cordinata -+
float distanceGreenNP;              //Distanza verde -+
float distancePurpleNP;             //Distanza viola -+
float debris1[2];                   //Array del debris in alto a sinistra
float debris17[2];
float puntoNN[2];                   //Coordinata --
float distanceGreenNN;              //Distanza verde --
float distancePurpleNN;             //Distanza viola --
float debris4[2];                  //Array del debris in alto a sinistra
float debris20[2];
float puntoPN[2];                   //Coordinata +-
float distanceGreenPN;              //Distanza verde +-
float distancePurplePN;             //Distanza viola +-
int t;                              //Contatore del tempo
int PreHook;                         //Contatore di volte nella posizione di aggancio
int s;
int temp;

float position1[3];                 //prima posizione del satellite blu (pre-debris)
float position2[3];                 // seconda  (debris)
float position3[3];                 //posizionamento di fronte al satellite rosso
float position4[3];                 // posizionamento per aggancio

float positionF[3];                 //Posizione di aggancio
float destination[3];



float distanceRedSphere;            //distanza tra i due satelliti


bool flagStop;                       //flag per fermare i cazzo di "true" e "false"
bool flagDebris;                    //flag per il controllo degli if per il movimento attraverso i debris
bool flagPosition1;
bool flagRendezvous;
bool ReadytoHook;

void init(){
    //INIZIALIZZAZIONE DELLE VARIABILI, ARRAY, MATRICI ECC.
    temp = 0;
    t = 0;                                          //Inizializzazione del tempo
    PreHook = 0;
    
    flagStop = true;                        //Una volta completato la fase di hook togliamo il coso che ci dice vero o falso
    flagDebris = true;
    flagPosition1 = false;                  //i flag ci servono a poter decidere, una volta raggiunta una posizione, tramite un if di muoverci in maniera differente.
    flagRendezvous = false;
    ReadytoHook =  false;

    api.getMyZRState(myState);
	api.getOtherZRState(otherState);        //otteniamo lo stato dei due satelliti

	for(int i = 0; i < 3; i++){     //scriviamo le posizioni negli array appositi
	    myPos[i] = myState[i];
	    otherPos[i] = otherState[i];
	}
   
    game.getDebris(debris);
    
    position3[0]=0;
    position3[1]=otherPos[1]+2*0.17095;       //-0.17;//-0.15327;//-0.17;         //posizione di fronte al meteorite rosso.
    position3[2]=0.228;
    
    position4[0]=myState[6];
    position4[1]=myState[7];
    position4[2]=myState[8];
    
    destination[0] = 0.0;
	destination[0] = 1.0;
	destination[0] = 0.0;
    
	searchPath();
	
	

}



void loop(){
    api.getMyZRState(myState);
	api.getOtherZRState(otherState);        //otteniamo lo stato dei due satelliti
	

	for(int i = 0; i < 3; i++){     //scriviamo le posizioni negli array appositi
	    myPos[i] = myState[i];
	    otherPos[i] = otherState[i];
	}


    position3[0] = -0.11236;                    //otherPos[0];         //aggiorniamo ogni volta le coordinate x e z, in modo da rimanere sempre affacciati di fronte al satellite rosso
    position3[2] = 0.21397;                    //otherPos[2];


    /*  
        Qui procediamo alla serie di movimenti necessari al superamento dei debris, di conseguenza
        ci allineamo prima del campo di debris
        lo superiamo attaccati ai bordi per non incorrere nella penalità
    */

    if(flagDebris){
        api.setPositionTarget(position1);

        if(hasReachedPos(position1)) flagPosition1 = true;

        if(flagPosition1){
            api.setPositionTarget(position2);
        }
        
        if(hasReachedPos(position2)) {
            flagDebris = false;
            flagRendezvous = true;
        }
        
    }
    
    
    /*
        inseriremo qua i procedimenti per la fase rendezvous    
    */
    
    if(flagRendezvous)
    {             //una volta terminata la fase di debris ci spostiamo a quella di rendezvous
        
        
        if(flagStop){
        DEBUG(("%s", game.getRendezvous() ? "true" : "false" ));
        position3[0]=position3[0]+0.0566;
        
        api.setPositionTarget(position3);
        api.setAttitudeTarget(position4);
        
        if (game.checkRendezvous()) game.completeRendezvous();
        }
            if (game.getRendezvous())
    	    {

        	        position4[0] = 0;
        	        position4[1] = -1;
        	        position4[2] = 0;
        	        api.setAttitudeTarget(position4);
        	        api.getOtherZRState(otherState);
        	        positionF[0] = otherState[0] - 0.0110;
        	        positionF[1] = otherState[1] + 0.3373;
        	        positionF[2] = otherState[2] + 0.026;
        	        
        	        api.setPositionTarget(positionF);
        	        if(game.getHooked())
        	        {
        	            flagStop = false;
        	            api.getMyZRState(myState);
        	            temp ++;
        	            if(temp>=2){
        	            position4[0]=1;
        	            position4[1]=0; 
        	            position4[2]=0; 
        	            position3[2]= -0.15; 
        	            position3[1]= 10.0; 
        	            position3[0]= -1.0;//positionF[0]-0.3; 
        	            
        	            api.setPositionTarget(position3);
        	            api.setAttitudeTarget(position4);
        	            
        	            if (s>=11){
        	            
        	                position3[2]= -0.15;
        	                position3[1]= 10.0; 
        	                position3[0]= -1;
        	                /*position3[2]= -0.3; 
        	                position3[1]= -0.4; 
        	                position3[0]= -0.3;*/ 
        	                api.setAttRateTarget(position4);
        	                api.setPositionTarget(position3);
        	                api.setAttitudeTarget(position4);
        	                
        	            }else if(s>=40){
        	                position3[2]= -0.3; 
        	                position3[1]= -0.4; 
        	                position3[0]= -0.3; 
        	                api.setAttRateTarget(position4);
        	                api.setPositionTarget(position3);
        	                api.setAttitudeTarget(position4);
        	            }
        	            if(s==6) 
        	            
        	            api.setAttRateTarget(position4); 
        	            s++;
        	            }
        	            
        	            
        	        }
    	    	
    	    }
	    
    }
    
        
  
    distanceRedSphere = distanceBetween3D(myPos, otherPos);         //aggiorniamo ogni volta la distanza tra i due satelliti
    
    //DEBUG(("distanceRedSphere: %f", distanceRedSphere));


	t++;
	
	/*if (game.checkRendezvous())
	    {
	        position3[0]=otherPos[0]-0.01;
	        position3[2]=position3[2]+0.01;
	        api.setPositionTarget(position3);
	    }*/
	
	 
	
    
	
}


float distanceBetween3D(float pos1[], float pos2[]){      //calcoliamo la distanza tra le due posizioni inserite tra i parametri in tre dimensioni
    float arr[3];

    mathVecSubtract(arr, pos1, pos2, 3);
    return mathVecMagnitude(arr, 3);
}

float distanceBetween2D(float pos1[], float pos2[]){      //calcoliamo la distanza tra le due posizioni inserite tra i parametri in due dimensioni
    float arr[2];

    mathVecSubtract(arr, pos1, pos2, 2);
    return mathVecMagnitude(arr, 2);
}

bool hasReachedPos(float pos[]){    //ritorna se il satellite ha raggiunto o meno un punto
    if(distanceBetween3D(pos, myPos) < 0.01){   //si può rendere più preciso
       return true;
    } else {
       return false;
    }
}

void moveFast(float pos[]){     //funzione per muoverci in un punto in maniera più veloce possibile, TODO: da ottimizzare

    float vec[3];
    mathVecSubtract(vec, pos, myPos, 3);    //sembra ci sia un problema con questa funzione, tale che se invertiamo pos e myPos abbiamo un comportamento inaspettato

    float distance = mathVecMagnitude(vec, 3);

    DEBUG(("DISTANZA: %f, quella vera %f", distance, distanceBetween3D(pos, myPos)));

    if(distance > 0.4) {
        api.setVelocityTarget(vec);         //non funzionante
        DEBUG(("CORRO"));
    }
    else {
        api.setPositionTarget(pos);
        DEBUG(("NEVVERO"));
    }
}


void searchPath(){
    
	/*
	Il procedimento utilizzato per passare attraverso ai debris è il seguente.
	nella prima fase inizializziamo le posizioni che riguardano il tragitto che il satellite deve fare, passando per il bordo per andare dall'altra parte del campo di debris.
	Inizializziamo i punti necessari al calcolo delle distanze.
	calcoliamo le distanze.
	decidiamo con un if la strada da percorrere e le coordinate.
	*/

    //Inizializzazione del punto ++                 //TODO: usare delle matrici per tutti sti punti e debris.
    puntoPP[0] =  0.60;                                //x
	puntoPP[1] =  0.60;                                //z

	//Inizializzazione del punto -+
    puntoNP[0] = -0.60;                               //x
	puntoNP[1] =  0.60;                               //z           //TODO: da fare in tre dimensioni. (forse)

	//Inizializzazione del punto -+
    puntoNN[0] = -0.60;                               //x
 	puntoNN[1] = -0.60;                               //z

	//Inizializzazione del punto -+
    puntoPN[0] =  0.60;                               //x
	puntoPN[1] = -0.60;                               //z

	//Inizializzazione dei debris verdi e viola esterni
	debris16[0] = debris[15][0];                    //x
	debris16[1] = debris[15][2];                    //z

	debris32[0] = debris[31][0];                    //x
    debris32[1] = debris[31][2];                    //z

	debris13[0] = debris[12][0];                    //x
	debris13[1] = debris[12][2];                    //z

	debris29[0] = debris[28][0];                    //x
	debris29[1] = debris[28][2];                    //z

	debris1[0] = debris[0][0];                    //x
	debris1[1] = debris[0][2];                    //z

	debris17[0] = debris[16][0];                  //x
    debris17[1] = debris[16][2];                  //z

	debris4[0] = debris[3][0];                    //x
	debris4[1] = debris[3][2];                    //z

	debris20[0] = debris[19][0];                  //x
    debris20[1] = debris[19][2];                  //z

	distanceGreenPP  = distanceBetween2D(puntoPP, debris16) +0.03;   // distanza(puntoPP,debris16, 'g');
	distancePurplePP = distanceBetween2D(puntoPP, debris32) +0.045;  // distanza(puntoPP,debris16, 'p');

	distanceGreenNP  = distanceBetween2D(puntoNP, debris13) +0.03;
	distancePurpleNP = distanceBetween2D(puntoNP, debris29) +0.045;

	distanceGreenNN  = distanceBetween2D(puntoNN, debris1) +0.03;
	distancePurpleNN = distanceBetween2D(puntoNN, debris17) +0.045;

	distanceGreenPN  = distanceBetween2D(puntoPN, debris4) +0.03;
	distancePurplePN = distanceBetween2D(puntoPN, debris20) +0.045;          

	//Controllo della distanza dai punti
	if (distanceGreenPP > 0.155 && distancePurplePP > 0.155) {
	    position1[0] = 0.63f;
	    position1[1] = 0.75f;                                   //position1 corrisponde alle coordinate davanti allo spazio dove dobbiamo passare
	    position1[2] = 0.62f;
	    
	    position2[0] =  0.60f;                        
        position2[1] = -0.17f;                                  //position2 alle coordinate dall'altra parte del campo di debris.
        position2[2] =  0.60f;                          
	}

	else if (distanceGreenNP > 0.155 && distancePurpleNP > 0.155) {         //TODO: qui mi affido a gerardi per questi if.
	    position1[0] = -0.60f;
	    position1[1] =  0.75f;
	    position1[2] =  0.60f;
	   
	    position2[0] = -0.60f;                         
        position2[1] = -0.17f;              //0.17 per allinearci al posizionamento di fronte al satellite rosso peri l rendezvous             
        position2[2] =  0.60f;                       
	}

	else if (distanceGreenNN > 0.155 && distancePurpleNN > 0.155){
	    position1[0] = -0.60f;
	    position1[1] =  0.75f;
	    position1[2] = -0.60f;                  
	  
	    position2[0] = -0.60f;                          
        position2[1] = -0.17f;
        position2[2] = -0.60f;                         
	}

	else if(distanceGreenPN > 0.155 && distancePurplePN > 0.155){
	    position1[0] =  0.60f;
	    position1[1] =  0.75f;
	    position1[2] = -0.60f;
	  
	    position2[0] =  0.60f;                        
        position2[1] = -0.17f;                          
        position2[2] = -0.60f;                        
	} else DEBUG(("NON TROVATO"));

}
