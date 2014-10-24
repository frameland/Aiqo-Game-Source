//
//  utility.cpp
//  Qpt
//
//  Created by Markus on 03.05.13.
//
//

#include "utility.h"
using namespace std;

string getAssetFolder()
{
    string dummyPath = ci::app::getAssetPath("dummy.png").string();
    int lastSlash = dummyPath.find_last_of("/");
    assert(lastSlash > 0);
    string assetFolder = dummyPath.substr(0, lastSlash+1);
    return assetFolder;
}
