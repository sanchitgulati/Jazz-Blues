//
//  FmodHelper.cpp
//  jazz
//
//  Created by Sanchit Gulati on 18/01/15.
//
//

#include "FmodHelper.h"
using namespace cocos2d;
void (*Common_Private_Error)(FMOD_RESULT);


bool FmodHelper::instanceFlag = false;
FmodHelper* FmodHelper::instance = NULL;

bool ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        if (Common_Private_Error)
        {
            Common_Private_Error(result);
        }
        log("FMOD error %d - %s", result, FMOD_ErrorString(result));
        return false;
    }
    return true;
}

FmodHelper* FmodHelper::getInstance()
{
    if(instanceFlag)
    {
        return FmodHelper::instance;
    }
    else
    {
        FmodHelper *pRet = new FmodHelper();
        if (pRet && pRet->init())
        {
            FmodHelper::instance = pRet;
            instanceFlag = true;
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
}

bool FmodHelper::init()
{
    int flag = true;
    
    FMOD_RESULT result;
    system = nullptr;
    result = Studio::System::create(&system); // Create the Studio System object.
    if (result != FMOD_OK)
    {
        flag = false;
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }
    // Initialize FMOD Studio, which will also initialize FMOD Low Level
    result = system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        flag = false;
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }
    
    
//    auto bank = FileUtils::getInstance()->fullPathForFilename("fmod/Master Bank.bank");
//    auto strings = FileUtils::getInstance()->fullPathForFilename("fmod/Master Bank.strings.bank");
    auto bank = FileUtils::getInstance()->fullPathForFilename("fmod/master.bank");
    auto strings = FileUtils::getInstance()->fullPathForFilename("fmod/master.strings.bank");
    
    FMOD::Studio::Bank* masterBank = NULL;
    flag = ERRCHECK( system->loadBankFile(bank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    
    FMOD::Studio::Bank* stringsBank = NULL;
    flag = ERRCHECK( system->loadBankFile(strings.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
    
    
    FMOD::Studio::ID guid;
    system->lookupID("bus:/", &guid);
    system->getMixerStrip(&guid, FMOD_STUDIO_LOADING_MODE::FMOD_STUDIO_LOAD_BEGIN_NOW, &mixer);
    
    
    return flag;
}


bool FmodHelper::loadEvent(std::string eventName)
{
    
    bool flag = true;
    
    //    auto eventString = StringUtils::format("event:/%s",eventName.c_str());
    auto eventString = StringUtils::format("event:/%s",eventName.c_str());
    FMOD::Studio::ID playID = {0};
    
    flag = ERRCHECK( system->lookupID(eventString.c_str(), &playID) );
    
    FMOD::Studio::EventDescription* playDescription = NULL;
    flag = ERRCHECK( system->getEvent(&playID, FMOD_STUDIO_LOAD_BEGIN_NOW, &playDescription) );
    
    flag = ERRCHECK( playDescription->loadSampleData() );
    
    FMOD::Studio::EventInstance* eventInstance = NULL;
    flag = ERRCHECK( playDescription->createInstance(&eventInstance) );
    
    events.push_back(std::make_pair(eventName, eventInstance));
    
    
    //    log("loading %s and result was %d",eventName.c_str(),flag);
    
    return flag;
}

bool FmodHelper::playEvent(std::string eventName)
{
    bool flag = false;
    std::vector<std::pair<std::string,Studio::EventInstance*>>::const_iterator it;
    for(it= events.begin(); it != events.end(); it++)
    {
        //        log("")
        if( strcmp((*it).first.c_str(),eventName.c_str()) == 0)
        {
            Studio::EventInstance* eventInstance = (*it).second;
            auto f = ERRCHECK(eventInstance->start());
            //            log("played %s and was sucess %d",eventName.c_str(),f);
            if(f)
                return true;
        }
    }
    return flag;
}

bool FmodHelper::stopEvent(std::string eventName,bool fade)
{
    bool flag = false;
    std::vector<std::pair<std::string,Studio::EventInstance*>>::const_iterator it;
    for(it= events.begin(); it != events.end(); it++)
    {
        if( strcmp((*it).first.c_str(),eventName.c_str()) == 0)
        {
            Studio::EventInstance* eventInstance = (*it).second;
            bool f;
            if(fade)
                f = ERRCHECK(eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE));
            else
                f = ERRCHECK(eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
            if(f)
                return true;
        }
    }
    return flag;
}


bool FmodHelper::changeParam(std::string eventName,std::string paramName,float val)
{
    bool flag = false;
    std::vector<std::pair<std::string,Studio::EventInstance*>>::const_iterator it;
    for(it= events.begin(); it != events.end(); it++)
    {
        if( strcmp((*it).first.c_str(),eventName.c_str()) == 0)
        {
            Studio::EventInstance* eventInstance = (*it).second;
            FMOD::Studio::ParameterInstance* mileParam = NULL;
            ERRCHECK( eventInstance->getParameter(paramName.c_str(), &mileParam) );
            return ERRCHECK( mileParam->setValue(val) );
        }
    }
    return flag;
}

void FmodHelper::update()
{
    system->update();
}

void FmodHelper::mute()
{
    mixer->setMute(true);
}

void FmodHelper::unmute()
{
    mixer->setMute(false);
}