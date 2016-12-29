#ifndef Kinematics99_h
#define Kinematics99_h

// Initialize Body Commands
float fPitch    = 0;           // Body Pitch (rad)
float fYaw      = 0;           // Body Yaw (rad)

// Function Protoypes
void Kinematics(float x0, float y0, float z0, float *X, float *Y, float *Z, float *Theta1, float *Theta2, float *Theta3);
void InverseKinematics(float x0, float y0, float z0, float x2, float y2, float z2,
                       float *Theta1, float *Theta2, float *Theta3);
void SingleRotation(float Yaw, float *X, float *Y, float *Z);                       
void DoubleRotation(float Pitch, float Yaw, float H, float *X, float *Y, float *Z);
  
/*********************************************************************************************
 * Rotation about Yaw Axis
 *********************************************************************************************/
void SingleRotation(float Yaw, float *X, float *Y, float *Z) {
  int i;
  
  for (i = 0; i <= 3; i++) {
    X[i] =  cos(Yaw) * X[i] + sin(Yaw) * Y[i];
    Y[i] = -sin(Yaw) * X[i] + cos(Yaw) * Y[i];
    Z[i] =  Z[i];
  }
}
/*********************************************************************************************
 * Rotation about Pitch/Yaw Axis and Height
 *********************************************************************************************/
void DoubleRotation(float Pitch, float Yaw, float H, float *X, float *Y, float *Z) {
  int i;
  
  for (i = 0; i <= 3; i++) {
    X[i] =  cos(Pitch) * cos(Yaw) * X[i] + sin(Yaw) * Y[i] + cos(Yaw) * sin(Pitch) * Z[i];
    Y[i] = -cos(Pitch) * sin(Yaw) * X[i] + cos(Yaw) * Y[i] - sin(Pitch) * sin(Yaw) * Z[i];
    Z[i] = -sin(Pitch) * X[i]                          + cos(Pitch) * Z[i]            + H;
  }
}
/*********************************************************************************************
 * Calculate Inverse Kinematics
 *********************************************************************************************/
float L2sq = L2 * L2;
float L1sq = L1 * L1;

void InverseKinematics(float x0, float y0, float z0, float x2, float y2, float z2,
                       float *Theta1, float *Theta2, float *Theta3) {
  float alphax;
  float alphaxsq;
  float alphaz;
  float alphazsq;
  float fErrChk;
                         
  // Solve for Theta1
  Theta1[0] = atan2((y2 - y0), (x2 - x0));

  // Solve for Theta2
  alphax = (x2 - x0) / cos(Theta1[0]) - L0 * cos(Phi);
  alphaxsq = alphax * alphax;
  alphaz = ((z2 - z0) - L0 * sin(Phi)) / L2;
  alphazsq = alphaz * alphaz;


//  Theta2[0]    = ((alphaxsq + L2sq * alphazsq) + L1sq - L2sq);
//  Theta2[0]    = Theta2[0] / (2 * L1 * sqrt(alphaxsq + L2sq * alphazsq));
//  Theta2[0]    = acos(Theta2[0]);
//  Theta2[0]    = Theta2[0] + atan2(L2 * alphaz, alphax);
  Theta2[0]    = acos(((alphaxsq + L2sq * alphazsq) + L1sq - L2sq) / (2 * L1 * sqrt(alphaxsq + L2sq * alphazsq)))
                 + atan2(L2 * alphaz, alphax);

  // Solve for Theta 3
  Theta3[0] = alphaz - L1 * sin(Theta2[0]) / L2;
  Theta3[0] = asin(Theta3[0]);
  Theta3[0] = Theta3[0] - Theta2[0];

  // Check for Alternate Solution
  float X[4] = {0, 0, 0, 0};
  float Y[4] = {0, 0, 0, 0};
  float Z[4] = {0, 0, 0, 0};
  Kinematics(x0, y0, z0, &X[0], &Y[0], &Z[0], Theta1, Theta2, Theta3);

  fErrChk = (X[3] - x2)*(X[3] - x2) + (Y[3] - y2)*(Y[3] - y2) + (Z[3] - z2)*(Z[3] - z2);
  if (fErrChk > 1e-6) {
    Theta3[0] = PI + asin(-alphaz + L1 * sin(Theta2[0]) / L2) - Theta2[0];
  }

  // Ensure Solution is between -pi and pi
  if (Theta3[0] >= PI) {
    Theta3[0] = Theta3[0] - 2 * PI;
  }
  else if (Theta3[0] <= -PI) {
    Theta3[0] = Theta3[0] + 2 * PI;
  }
}
/*********************************************************************************************
 * Calculate Kinematics
 *********************************************************************************************/
void Kinematics(float x0, float y0, float z0, float *X, float *Y, float *Z, float *Theta1, float *Theta2, float *Theta3) {
  float xp, yp, zp, x1, y1, z1, x2, y2, z2;
  
  xp  = x0 + L0 * cos(Theta1[0]) * cos(Phi);
  yp  = y0 + L0 * sin(Theta1[0]) * cos(Phi);
  zp  = z0 + L0 * sin(Phi);

  x1  = xp + L1 * cos(Theta1[0]) * cos(Theta2[0]);
  y1  = yp + L1 * sin(Theta1[0]) * cos(Theta2[0]);
  z1  = zp + L1 * sin(Theta2[0]);

  x2  = x1 + L2 * cos(Theta1[0]) * cos(Theta2[0] + Theta3[0]);
  y2  = y1 + L2 * sin(Theta1[0]) * cos(Theta2[0] + Theta3[0]);
  z2  = z1 + L2 * sin(Theta2[0] + Theta3[0]);

  X[0] = x0; X[1] = xp; X[2] = x1; X[3] = x2;
  Y[0] = y0; Y[1] = yp; Y[2] = y1; Y[3] = y2;
  Z[0] = z0; Z[1] = zp; Z[2] = z1; Z[3] = z2;
}

#endif /*Kinematics99*/  
