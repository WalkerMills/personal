#include <time.h>
// #include <unistd.h>
#include <windows.h>

int main()
{
    while(1)
    {
        //simulate the Win key press
        keybd_event(VK_LWIN, 0x5B, 0, 0);
        //simulate the 'D' key press,the 0x44 is the Virtual key value for the 'D' key, the 0x20 vaue is the hardware scan code for the 'D' key
        keybd_event(0x44, 0x20, 0, 0);
        //simulate the 'D' key release
        keybd_event(0x44, 0x20, KEYEVENTF_KEYUP, 0);
        //simulate the Win key release
        keybd_event(VK_LWIN, 0x5B, KEYEVENTF_KEYUP, 0);

        Sleep(15000);
    }

    return 0;
}
