#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    for (;;)
    {
        //string s("What is ISIS? ISIS, or the Islamic State in Iraq and Syria, is a group of radical fundamentalist anti-Western Islamists who believe that Muslims must follow strict Sharia law and forsake cultural perversions such as capitalistic greed and social liberty. A very moralistic lot, ISIS believes that human beings should be guided by a strict and literal reading of the Koran, which is to say, what they interpret it to mean. The highest calling is jihad, to do Allah's work on earth, for which a soldier will be rewarded in heaven. Making the ultimate sacrifice of his life will yield the greatest reward. Just as in Christianity, whatever happens on this planet is relatively insignificant in the grand scheme of things. Eternity lasts forever, while life on earth is transitory. After the 2003 invasion of Iraq, radical Islamists coalesced under the name ISIS and collaborated with Al Qaeda. Both Iraqis and foreigners joined the ranks of the insurgents as militants and civilians were being slaughtered by the Western occupiers. ISIS eventually separated from Al Qaeda (apparently not radical enough) and went on to establish outposts in other lands, including Syria, where their aim has been to oust President Bashar al-Assad and establish a caliphate.In late 2012 and 2013, the CIA covertly furnished 600 tons of weapons to appropriately vetted moderate rebels. The result was a massive takeover by ISIS of large swaths of territory in both Syria and Iraq. In 2014, ISIS declared that it had established a caliphate to govern over all Muslims. In conclusion, ISIS is an enemy created by the US government through intervention abroad and the military empowerment of outsiders. Like Saddam Hussein and Osama bin Laden before them, ISIS, having been funded by US taxpayers, now serves as the pretext for more war in the Middle East.");
        string s("ball cat dog ball.");
        string target;
        string replacement;

        cout << "Original Text: " << s << endl;
        cout << endl;
        cout << "Enter a target word OR enter '!wq' to exit." << endl;
        cin >> target;
        cout << "Now, enter a replacement word." << endl;
        cin >> replacement;

        long childPID;
        childPID = fork();

        if (childPID > 0)
        {
            wait(0);
        }
        else if (childPID == 0)
        {
            cout << endl;
            cout << "======== Child Process ========" << endl;

            int match = 0;
            while (match == 0)
            {
                cout << "." << endl;
                size_t nPos = s.find(target, 0);
                while (nPos != string::npos)
                {
                    match++;
                    s.replace(nPos, target.size(), replacement);
                    nPos = s.find(target, nPos+1);
                }
            }

            cout << "Matches found: " << match << endl;
            cout << "New Text: " << s << endl;
            cout << endl;
        }

        string answer;
        cout << "Repeat? ('y' for yes, '!wq' for no)" << endl;
        cin >> answer;
        if (answer == "!wq")
            break;
        cout << "------------------------------------------------------" << endl;
        cout << endl;

    }

    return 0;
}
