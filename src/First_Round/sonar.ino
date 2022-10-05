int autoSteer(char h)
{
  int minDist = 2, maxDist, minAng, maxAng;
  int distance, wallPos, err, lasterr = 0, P, D, ang;
  maxDist = (wallDist - minDist) * 2;
  minAng = mid_pos - max_steering;
  maxAng = mid_pos + max_steering;
  if (h == 'R')distance = sonarR.ping_cm();
  else distance = sonarL.ping_cm();
  if (distance < minDist && distance != 0)distance = minDist;
  if (distance > maxDist || distance == 0)distance = maxDist;
  //Serial.println(distance);
  if(h == 'L')ang = constrain(map(distance, minDist, maxDist, maxAng, minAng), minAng, maxAng);
 else ang = constrain(map(distance, minDist, maxDist, minAng, maxAng), minAng, maxAng);

  myservo.write(ang);
  //delay(30);
  return constrain(map(ang, mid_pos-max_steering, mid_pos+max_steering, -40,40),-40,40);
}
