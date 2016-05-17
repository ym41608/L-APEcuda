#ifndef PARAMETER_H
#define PARAMETER_H

// class parameter
class parameter {
  public:
    void shrinkNet(const float& factor) {
      txS *= factor;
      tyS *= factor;
      tzS *= factor;
      rxS *= factor;
      rz0S *= factor;
      rz1S *= factor;
      delta *= factor;
    };
    
    // about pose net
    float delta;
    float tzMin, tzMax;
    float rxMin, rxMax;
    float rzMin, rzMax;
    float txS, tyS, tzS, rxS, rz0S, rz1S;
    
    // about camera
    float Sfx, Sfy;
    float Px, Py;
    
    // about marker
    int mDimX, mDimY;
    float markerDimX, markerDimY;
    
    // about img
    int iDimX, iDimY;
};

#endif