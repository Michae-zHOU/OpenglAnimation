//
//  Channel.cpp
//  cse169
//
//  Created by Ziyao Zhou on 4/22/16.
//
//

#include "Channel.h"

bool Channel::Load(Tokenizer* token){
    while(1){
        char temp[256];
        token->GetToken(temp);
        
        if(strcmp(temp, "extrapolate") == 0){
            token->GetToken(temp);
            
            if(strcmp(temp, "constant") == 0){
                modebefore = constant;
            }
            else if(strcmp(temp, "linear") == 0){
                modebefore = linear;
            }
            else if(strcmp(temp, "cycle") == 0){
                modebefore = cycle;
            }
            else if(strcmp(temp, "cycle_offset") == 0){
                modebefore = cycle_offset;
            }
            else if(strcmp(temp, "bounce") == 0){
                modebefore = bounce;
            }
            
            token->GetToken(temp);
            
            if(strcmp(temp, "constant") == 0){
                modeafter = constant;
            }
            else if(strcmp(temp, "linear") == 0){
                modeafter = linear;
            }
            else if(strcmp(temp, "cycle") == 0){
                modeafter = cycle;
            }
            else if(strcmp(temp, "cycle_offset") == 0){
                modeafter = cycle_offset;
            }
            else if(strcmp(temp, "bounce") == 0){
                modeafter = bounce;
            }
        }
        else if(strcmp(temp, "keys") == 0){
            keysize = token->GetInt();
            
            KeyFramelist = new Keyframe[keysize];
            token->GetToken(temp);
            
            for(int i=0; i<keysize; i++){
               
                KeyFramelist[i].Time = token->GetFloat();
                KeyFramelist[i].Value = token->GetFloat();
                
                if(i==0)
                    time_start = KeyFramelist[i].Time;
                if(i==keysize-1)
                    time_end = KeyFramelist[i].Time;
                
                char in[256];
                char out[256];
                
                token->GetToken(in);
                token->GetToken(out);
                
                if(strcmp(in, "flat")==0){
                    KeyFramelist[i].RuleIn = Keyframe::flat;
                }
                else if(strcmp(in, "linear")==0){
                    KeyFramelist[i].RuleIn = Keyframe::linear;
                }
                else if(strcmp(in, "smooth")==0){
                    KeyFramelist[i].RuleIn = Keyframe::smooth;
                }
                else if(strcmp(in, "fixed")==0){
                    KeyFramelist[i].RuleIn = Keyframe::fixed;
                }
                
                if(strcmp(out, "flat")==0){
                    KeyFramelist[i].RuleOut = Keyframe::flat;
                }
                else if(strcmp(out, "linear")==0){
                    KeyFramelist[i].RuleOut = Keyframe::linear;
                }
                else if(strcmp(out, "smooth")==0){
                    KeyFramelist[i].RuleOut = Keyframe::smooth;
                }
                else if(strcmp(out, "fixed")==0){
                    KeyFramelist[i].RuleOut = Keyframe::fixed;
                }
            }
        }
        else if(strcmp(temp, "}") == 0){
            Precompute();
            return true;
        }
        else
            token->SkipLine();
    }
}

void Channel::SetTangent(){
    for(int i=0; i<keysize; i++){
        if(KeyFramelist[i].RuleIn == Keyframe::fixed){
            KeyFramelist[i].TangentIn = KeyFramelist[i].Value;
        }
        if(KeyFramelist[i].RuleOut == Keyframe::fixed){
            KeyFramelist[i].TangentOut = KeyFramelist[i].Value;
        }
        if(KeyFramelist[i].RuleIn == Keyframe::flat){
            KeyFramelist[i].TangentIn = 0;
        }
        if(KeyFramelist[i].RuleOut == Keyframe::flat){
            KeyFramelist[i].TangentOut = 0;
        }
        if(KeyFramelist[i].RuleIn == Keyframe::linear){
            if(i==0){
                if(keysize == 1){
                    KeyFramelist[i].TangentIn = 0;
                }
                else{
                    KeyFramelist[i].TangentIn = (KeyFramelist[0].Value-KeyFramelist[keysize-1].Value)/(KeyFramelist[0].Time-KeyFramelist[keysize-1].Time);
                }
            }
            else{
                //KeyFramelist[i].TangentIn = (KeyFramelist[i].Value-KeyFramelist[i-1].Value)/(KeyFramelist[i].Time-KeyFramelist[i-1].Time);
                KeyFramelist[i].TangentIn = (KeyFramelist[i-1].Value-KeyFramelist[i].Value)/(KeyFramelist[i-1].Time-KeyFramelist[i].Time);
            }
        }
        if(KeyFramelist[i].RuleOut == Keyframe::linear){
            if(i==keysize-1){
                if(keysize == 1){
                    KeyFramelist[i].TangentOut = 0;
                }
                else{
                    KeyFramelist[i].TangentOut = (KeyFramelist[keysize-1].Value-KeyFramelist[0].Value)/(KeyFramelist[keysize-1].Time-KeyFramelist[0].Time);
                }
            }
            else{
                KeyFramelist[i].TangentOut = (KeyFramelist[i+1].Value-KeyFramelist[i].Value)/(KeyFramelist[i+1].Time-KeyFramelist[i].Time);
            }
        }
        if(KeyFramelist[i].RuleIn == Keyframe::smooth){
            if(i==0){
                if(keysize == 1){
                    KeyFramelist[i].TangentIn = 0;
                }
                else{
                    KeyFramelist[i].TangentIn = (KeyFramelist[0].Value-KeyFramelist[keysize-1].Value)/(KeyFramelist[0].Time-KeyFramelist[keysize-1].Time);
                }
            }
            else{
                //KeyFramelist[i].TangentIn = (KeyFramelist[i+1].Value-KeyFramelist[i-1].Value)/(KeyFramelist[i+1].Time-KeyFramelist[i-1].Time);
                KeyFramelist[i].TangentIn = (KeyFramelist[i-1].Value-KeyFramelist[i+1].Value)/(KeyFramelist[i-1].Time-KeyFramelist[i+1].Time);
            }
        }
        if(KeyFramelist[i].RuleOut == Keyframe::smooth){
            if(i==keysize-1){
                if(keysize == 1){
                    KeyFramelist[i].TangentOut = 0;
                }
                else{
                    KeyFramelist[i].TangentOut = (KeyFramelist[keysize-1].Value-KeyFramelist[0].Value)/(KeyFramelist[keysize-1].Time-KeyFramelist[0].Time);
                }
            }
            else{
                KeyFramelist[i].TangentOut = (KeyFramelist[i+1].Value-KeyFramelist[i-1].Value)/ (KeyFramelist[i+1].Time-KeyFramelist[i-1].Time);
            }
        }
    }
}

void Channel::Precompute(){
    SetTangent();
    for(int i=0; i<keysize-1; i++){
        float p0 = KeyFramelist[i].Value;
        float p1 = KeyFramelist[i+1].Value;
        float v0 = (KeyFramelist[i+1].Time-KeyFramelist[i].Time)*KeyFramelist[i].TangentOut;
        float v1 = (KeyFramelist[i+1].Time-KeyFramelist[i].Time)*KeyFramelist[i].TangentIn;
        KeyFramelist[i].A = 2*p0-2*p1+v0+v1;
        KeyFramelist[i].B = (-3)*p0+3*p1-2*v0-v1;
        KeyFramelist[i].C = v0;
        KeyFramelist[i].D = p0;
    }
    
    float p0 = KeyFramelist[keysize-1].Value;
    float p1 = KeyFramelist[0].Value;
    float v0 = (KeyFramelist[0].Time-KeyFramelist[keysize-1].Time)*KeyFramelist[keysize-1].TangentOut;
    float v1 = (KeyFramelist[0].Time-KeyFramelist[keysize-1].Time)*KeyFramelist[keysize-1].TangentIn;
    
    KeyFramelist[keysize-1].A = 2*p0-2*p1+v0+v1;
    KeyFramelist[keysize-1].B = (-3)*p0+3*p1-2*v0-v1;
    KeyFramelist[keysize-1].C = v0;
    KeyFramelist[keysize-1].D = p0;
}

float Channel::Evaluate(float time){
    if(time < time_start){
        if(modebefore == constant){
            return Evaluate(time_start);
        }
        else if(modebefore == linear){
            return KeyFramelist[0].Value+(time-KeyFramelist[0].Time)*KeyFramelist[0].TangentIn;
        }
        else if(modebefore == cycle){
            float t = time_start - time;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            t = time_end - t;
            
            return Evaluate(t);
        }
        else if(modebefore == cycle_offset){
            float diff = Evaluate(time_end) - Evaluate(time_start);
            
            float t = time_start - time;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            t = time_end - t;
            
            return (Evaluate(t) - (m+1)*diff);
        }
        else if(modebefore == bounce){
            float t = time_start - time;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            
            if(m%2 == 1)
                return Evaluate(t);
            else{
                t = time_end - t;
                return Evaluate(t);
            }
        }
    }
    else if(time > time_end){
        if(modeafter ==  constant){
            return Evaluate(time_end);
        }
        else if(modeafter == linear){
            return KeyFramelist[keysize-1].Value+(time-KeyFramelist[keysize-1].Time)*KeyFramelist[keysize-1].TangentOut;
        }
        else if(modeafter == cycle){
            float t = time-time_end;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            t+=time_start;
            
            return Evaluate(t);
        }
        else if(modeafter == cycle_offset){
            float diff = Evaluate(time_end) - Evaluate(time_start);
            
            float t = time-time_end;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            
            return (m+1)*diff+Evaluate(t);
            
        }
        else if(modeafter == bounce){
            float t = time-time_end;
            int m = t/(time_end - time_start);
            t -= (m*(time_end - time_start));
            
            if(m%2 == 1){
                t = time_end - t;
                return Evaluate(t);
            }
            else
                return Evaluate(t);
        }
    }

    for(int i=0; i<keysize; i++){
        if(time == KeyFramelist[i].Time)
            return KeyFramelist[i].Value;
        
        else if(time > KeyFramelist[i].Time && time < KeyFramelist[i+1].Time){
            float a = KeyFramelist[i].A, b = KeyFramelist[i].B, c = KeyFramelist[i].C, d = KeyFramelist[i].D;
        
            float span = KeyFramelist[i+1].Time - KeyFramelist[i].Time;
            float u = (time - KeyFramelist[i].Time)/span;
            float result = d+u*(c+u*(b+u*a));
            
            if(result == 0)
                return 0;
            else if( result < -100000)
                return 0;
            
            return result;
        }
        else{
            continue;
        }
    }
    return 0;
}