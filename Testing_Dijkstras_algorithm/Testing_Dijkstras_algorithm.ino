int route1[] = {0, 1, 3, 2, 0}; // Input route
int route2[0]; // Output route
int route2Index = 0;

void routeFix() {
  int count1 = sizeof(route1) / sizeof(route1[0]);

  // Matrix defining intermediate steps
  int matrix[6][6][4] = {
    //  0        1        2        3        4        5   
    { {-1},     {6},     {6},     {2, 6},  {-1},    {7, 1, 6} }, // 0
    { {6},      {-1},    {6},     {2, 6},  {0, 6},     {7} },   // 1
    { {6},      {6},     {-1},    {-1},     {0, 6},     {7, 1, 6} }, // 2
    { {6, 2},   {6, 2},  {-1},     {-1},    {7},     {7} },     // 3
    { {-1},      {6, 0}, {6, 0}, {7}, {-1},    {7} },     // 4
    { {6, 1, 7},   {7},     {6, 1, 7}, {7},     {7},     {-1} }     // 5
  };

  route2[0] = route1[0];
  Serial.print(route2[0]);
  Serial.print(" ");
  
  for(int a=0; a<count1; a++){
    while(matrix[route1[a]][route1[a+1]][0] != -1) {
      int array[] = matrix[route1[a]][route1[a+1]][4];
      int count2 = sizeof(array) / sizeof(array[0]);
      for(b=0; b<count2; b++){
        route2[b] = array[b];
        Serial.print(route2[b]);
        Serial.print(" ");
      }     
    }
    a++;
    int count3 = sizeof(route2) / sizeof(route2[0]);
    count3++;
    route2[count3] = route1[a];
    Serial.print(route2[count3]);
    Serial.print(" ");
  }
  Serial.println();

void setup() {
  Serial.begin(9600);
  delay(500); // Allow Serial to stabilize
}

void loop() {
  routeFix();
  delay(1500);
}
