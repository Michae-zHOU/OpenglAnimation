//
//  Rig.h
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#ifndef RIG_H
#define RIG_H

#include <stdio.h>
#include "Skeleton.h"
#include "Skin.h"

class Rig {
private:
    bool skelVisible = true;
public:
    Skeleton* skel;
    Skin* skin;
    
    Rig(Skeleton*, Skin*);
    void Draw();
    void Update();
    
    void setSkelVisible(int);
};


#endif /* Rig_h */
