int autoSteer(char h) {
  int minDist = 2, maxDist, minAng, maxAng;
  int distance, wallPos, err, lasterr = 0, P, D, ang;
  maxDist = (wallDist - minDist) * 2;
  //  maxDist = 76;
  minAng = mid_pos - max_steering;
  maxAng = mid_pos + max_steering;
  if (h == 'R')distance = sonarR.ping_cm();
  else distance = sonarL.ping_cm();
  if (distance == 0) distance = 200;
  //  Serial.print("min dist: ");
  //  Serial.print(minDist);
  //  Serial.print("  max dist: ");
  //  Serial.print(maxDist);
  //  Serial.print("min ang: ");
  //  Serial.print(minAng);
  //  Serial.print("  max ang: ");
  //  Serial.print(mid_pos);
  //  Serial.print("  Sonar: ");
  //  Serial.print(distance);
  if (distance < minDist && distance != 0)distance = minDist;
  if (distance > maxDist || distance == 0)distance = maxDist;
  Serial.print(distance);

  if (firstIntersection == true) {
    MPU();
    delay(2);
    int LD = sonarL.ping_cm();
    if (LD == 0 || LD > 100) {
      leftCount++;
      LD = MAX_DISTANCE;
    }
    int RD = sonarR.ping_cm();
    if (RD == 0 || RD > 100) {
      rightCount++;
      RD = MAX_DISTANCE;
    }
    if (leftCount > 10) {
      firstIntersection = false;
      dir = 'L';
    }
    if (rightCount > 10) {
      firstIntersection = false;
      dir = 'R';
    }
    int error = LD - RD;
    ang = constrain(map(error, -80, 80, 50, 140), 50, 140);
  } else {
    //  if (distance == 0 || distance > 100) distance = 300;
    if (h == 'R') {
      ang = constrain(map(distance, minDist, maxDist, maxAng, minAng), 48, 142);
      if (distance == maxDist)
        ang = 50;
    } else {
      ang = constrain(map(distance, minDist, maxDist, minAng, maxAng), 48, 142);
      if (distance == maxDist)
        ang = 142;
    }
  }
  myservo.write(ang);
  delay(10);
  return map(ang, mid_pos - max_steering, mid_pos + max_steering, -32, 40);
}
