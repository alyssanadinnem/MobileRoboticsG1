int route1[] = {0, 1, 3, 2, 0}; // Input route
int route2[20]; // Output route
int route2Index = 0;

void routeFix() {
  int count1 = sizeof(route1) / sizeof(route1[0]);

  // Matrix defining intermediate steps
  int matrix[6][6][4] = {
    //  0        1        2        3        4        5   
    { {-1},     {6},     {6},     {2, 6},  {-1},    {7, 1, 6} }, // 0
    { {6},      {-1},    {6},     {2, 6},  {6},     {7, 1} },   // 1
    { {6},      {6},     {-1},    {2},     {6},     {7, 1, 6} }, // 2
    { {6, 2},   {6, 2},  {3},     {-1},    {7},     {7} },     // 3
    { {4},      {6, 0, 4}, {6, 0, 4}, {7}, {-1},    {7} },     // 4
    { {4, 7},   {7},     {3, 7}, {7},     {7},     {-1} }     // 5
  };

  route2Index = 0;

  for (int i = 0; i < count1; i++) {
    int current = route1[i];
    int next = route1[i + 1];

    // Always add the current node to route2
    if (route2Index == 0 || route2[route2Index - 1] != current) {
      route2[route2Index++] = current;
    }

    // Insert intermediate nodes from matrix[current][next]
    if (matrix[current][next][0] != -1) {
      for (int j = 0; j < 4; j++) {
        if (matrix[current][next][j] == -1) break; // Stop at invalid entry

        // Prevent duplicate consecutive insertions
        if (route2Index == 0 || route2[route2Index - 1] != matrix[current][next][j]) {
          route2[route2Index++] = matrix[current][next][j];
        }
      }
    }
  }

  // Add the last node from route1
  if (route2[route2Index - 1] != route1[count1 - 1]) {
    route2[route2Index++] = route1[count1 - 1];
  }

  // Print route2
  Serial.print("Updated Route2: ");
  for (int i = 0; i < route2Index; i++) {
    Serial.print(route2[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  delay(500); // Allow Serial to stabilize
  routeFix();
}

void loop() {
  delay(2000);
}
