#ifndef SumoEdge_h
#define SumoEdge_h

//edge sensor input pins
int EDGE_L = 3;
int EDGE_R = 4;

//Edge Sample
#define EDGESAMPLE 3
#define EDGE_DETECT_THRESHOLD 2

void edge_init()
{
  pinMode(EDGE_L, INPUT);
  pinMode(EDGE_R, INPUT);
}

bool left_edge()
{   
  int edgeleftcount = 0;
  
  for(int x=0; x < EDGESAMPLE; x++) {
    if(digitalRead(EDGE_L) == HIGH) {
      edgeleftcount++;
    }
  }

  if(edgeleftcount >= EDGE_DETECT_THRESHOLD) return true;
  else return false;

  return false;
}

bool right_edge()
{ 
  int edgerightcount = 0;
  
  for(int x=0; x < EDGESAMPLE; x++) {
    if(digitalRead(EDGE_R) == HIGH) {
      edgerightcount++;
    }
  }
  
  if(edgerightcount >= EDGE_DETECT_THRESHOLD) return true;
  else return false;

  return false;
}

#endif // SumoEdge_h
