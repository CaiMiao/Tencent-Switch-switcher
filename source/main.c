#include <string.h>
#include <stdio.h>

#include <switch.h>

//See also libnx set.h.

int main(int argc, char **argv)
{
    u64 LanguageCode=0;
    SetLanguage Language=SetLanguage_ENUS;
    SetRegion RegionCode=SetRegion_JPN;
    bool bT=false;
    //SetSysPlatformRegion PlatformRegion=SetSysPlatformRegion_Global;

    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    printf("Tencent Switch switcher v0.1 by CaiMiao\n");
    printf("-------------------------\n");
    printf("DISCLAIMER: The author does not take any responsibility regarding potential\nBRICKs & BANs. Be wise and use at your own risk.\n");
    printf("-------------------------\n");
    printf("print current values...\n\n");

    Result rc = setInitialize();
    if (R_FAILED(rc)) printf("setInitialize() failed: 0x%x.\n", rc);

    rc = setsysInitialize();
    if (R_FAILED(rc)) printf("setsysInitialize() failed: 0x%x.\n", rc);

    if (R_SUCCEEDED(rc))
    {
        //Get system language.
        rc = setGetLanguageCode(&LanguageCode);
        if (R_FAILED(rc)) printf("setGetLanguageCode() failed: 0x%x.\n", rc);
    }

    if (R_SUCCEEDED(rc))
    {
        printf("LanguageCode: %s\n", (char*)&LanguageCode);

        // Convert LanguageCode to Language. Use this if you need IDs, not strings.
        rc = setMakeLanguage(LanguageCode, &Language);

        if (R_FAILED(rc)) printf("setMakeLanguage() failed: 0x%x.\n", rc);
    }

    if (R_SUCCEEDED(rc))
    {
        printf("Language (ID): %d\n", Language);

        // Get region code.
        rc = setGetRegionCode(&RegionCode);

        if (R_FAILED(rc)) printf("setGetRegionCode() failed: 0x%x.\n", rc);
    }

    if (R_SUCCEEDED(rc))
    {
        printf("RegionCode: %d\n", RegionCode);

        // Get bool of IsT (isTencent).
        rc = setsysGetT(&bT);

        if (R_FAILED(rc)) printf("setsysGetT() failed: 0x%x.\n", rc);
    }

    if (R_SUCCEEDED(rc)) printf("IsT (isTencent): %d\n", bT);

    printf("-------------------------\n");
    printf("Press UP to set safe Tencent params (changes language to zh-Hans)\n");
    printf("Press X to set safe NON-Tencent params (does not change language)\n");
    printf("Press Y to set English Language if you are gaijin\n");
    printf("Press + to quit\n\n");

    // Main loop
    while(appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // Your code goes here

        // padGetButtonsDown returns the set of buttons that have been newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Up) // set to T
        {
            setMakeLanguageCode(SetLanguage_ZHHANS, &LanguageCode);
            rc = setsysSetLanguageCode(LanguageCode);
            setGetLanguageCode(&LanguageCode);
            if (R_SUCCEEDED(rc)) printf("Success. LanguageCode: %s\n", (char*)&LanguageCode);
            rc = setsysSetT(true);
            setsysGetT(&bT);
            if (R_SUCCEEDED(rc)) printf("Success. IsT: %d\n", bT);
            rc = setsysSetRegionCode(SetRegion_CHN);
            setGetRegionCode(&RegionCode);
            if (R_SUCCEEDED(rc)) printf("Success. Region: %d\n", RegionCode);
            printf("Done. Restart to take effect.\n\n");
        }
        if (kDown & HidNpadButton_X) //set to non-T
        {
            rc = setsysSetT(false);
            setsysGetT(&bT);
            if (R_SUCCEEDED(rc)) printf("Success. IsT: %d\n", bT);
            rc = setsysSetRegionCode(SetRegion_HTK);
            setGetRegionCode(&RegionCode);
            if (R_SUCCEEDED(rc)) printf("Success. Region: %d\n", RegionCode);
            printf("Done. Restart to take effect.\n\n");
        }
        if (kDown & HidNpadButton_Y) // for sake
        {
            setsysGetT(&bT);
            if (bT == true) {
                printf("You will brick yourself when IsT = true. Aborted.\n");
            } else {
                setMakeLanguageCode(SetLanguage_ENUS, &LanguageCode);
                rc = setsysSetLanguageCode(LanguageCode);
                setGetLanguageCode(&LanguageCode);
                if (R_SUCCEEDED(rc)) printf("Success. LanguageCode: %s\n", (char*)&LanguageCode);
                rc = setsysSetRegionCode(SetRegion_USA);
                setGetRegionCode(&RegionCode);
                if (R_SUCCEEDED(rc)) printf("Success. Region: %d\n", RegionCode);
                printf("Done. Restart to take effect.\n\n");
            }
        }
        if (kDown & HidNpadButton_ZR) // nothing
        {
            printf("HIDDEN DEBUG BUTTON but nothing happens.\n");
        }

        if (kDown & HidNpadButton_Plus) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }

    setsysExit();
    setExit();
    consoleExit(NULL);
    return 0;
}
