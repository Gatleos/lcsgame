/*
This file is part of Liberal Crime Squad.                                           //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

#include <includes.h>
#include <externs.h>

/* base - activate sleepers */
void activate_sleepers(void) {
    vector<Creature *> temppool;

    // Comb the pool of Liberals for sleeper agents
    for(int p = 0; p < pool.size(); p++) {
        // Select only sleepers that can work
        if(pool[p]->alive == true &&
                pool[p]->flag & CREATUREFLAG_SLEEPER &&
                pool[p]->align == ALIGN_LIBERAL &&
                pool[p]->hiding == false &&
                pool[p]->clinic == false &&
                pool[p]->dating == false)
            temppool.push_back(pool[p]);
    }

    if(temppool.size() == 0)
        return;

    int page = 0;

    char str[80];

    do {
        erase();

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        printfunds(0, 1, "Money: ");


        move(0, 0);
        addstr("Activate Sleeper Agents");
        makedelimiter(1, 0);
        move(1, 4);
        addstr("CODE NAME");
        move(1, 25);
        addstr("JOB");
        move(1, 42);
        addstr("SITE");
        move(1, 57);
        addstr("ACTIVITY");

        int y = 2;

        for(int p = page * 9; p < temppool.size() && p < page * 9 + 9; p++) {
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(y, 0);
            addch((y - 2) / 2 + 'A');
            addstr(" - ");
            addstr(temppool[p]->name);

            move(y, 25);
            getrecruitcreature(str, temppool[p]->type);
            addstr(str);

            move(y + 1, 6);
            addstr("Effectiveness: ");

            /*move(y+1,28);
            set_color(COLOR_GREEN,COLOR_BLACK,0);
            addstr("-");
            set_color(COLOR_WHITE,COLOR_BLACK,0);
            addstr("-");
            set_color(COLOR_WHITE,COLOR_BLACK,1);
            addstr("-");
            set_color(COLOR_YELLOW,COLOR_BLACK,1);
            addstr("-");
            set_color(COLOR_MAGENTA,COLOR_BLACK,1);
            addstr("-");
            set_color(COLOR_RED,COLOR_BLACK,1);
            addstr("-"); */

            if(temppool[p]->infiltration > 0.8f) {
                set_color(COLOR_RED, COLOR_BLACK, 1);
                //move(y+1,33); addch('*');
                //move(y+1,37); addstr("Stalwart Arch-Conservative");
            } else if(temppool[p]->infiltration > 0.6f) {
                set_color(COLOR_MAGENTA, COLOR_BLACK, 1);
                //move(y+1,32); addch('*');
                //move(y+1,37); addstr("Trusted Conservative");
            } else if(temppool[p]->infiltration > 0.4f) {
                set_color(COLOR_YELLOW, COLOR_BLACK, 1);
                //move(y+1,31); addch('*');
                //move(y+1,37); addstr("Known Conservative");
            } else if(temppool[p]->infiltration > 0.2f) {
                set_color(COLOR_WHITE, COLOR_BLACK, 1);
                //move(y+1,30); addch('*');
                //move(y+1,37); addstr("Respected");
            } else if(temppool[p]->infiltration > 0.1f) {
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
                //move(y+1,29); addch('*');
                //move(y+1,37); addstr("Ineffective");
            } else {
                set_color(COLOR_GREEN, COLOR_BLACK, 0);
                //move(y+1,28); addch('*');
                //move(y+1,37); addstr("Known Liberal");
            }

            char num[10];
            itoa(static_cast<int>(temppool[p]->infiltration * 100), num, 10);
            addstr(num);
            addstr("%");

            move(y, 42);
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            addstr(location[temppool[p]->worklocation]->shortname);

            move(y, 57);
            // Let's add some color here...
            set_activity_color(temppool[p]->activity.type);
            getactivity(str, temppool[p]->activity);
            addstr(str);

            y += 2;
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(22, 0);
        addstr("Press a Letter to Assign an Activity.");
        move(23, 0);
        addpagestr();

        set_color(COLOR_WHITE, COLOR_BLACK, 0);

        refresh();

        int c = getch();
        translategetch(c);

        //PAGE UP
        if((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
            page--;

        //PAGE DOWN
        if((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT) && (page + 1) * 9 < temppool.size())
            page++;

        if(c >= 'a' && c <= 's') {
            int p = page * 9 + (int)(c - 'a');

            if(p < temppool.size())
                activate_sleeper(temppool[p]);
        }

        if(c == 10)
            break;
    } while(1);
}


void activate_sleeper(Creature *cr) {
    int state = 0;
    int choice = 0;
    char havedead = 0;

    do {
        erase();

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        printfunds(0, 1, "Money: ");

        move(0, 0);
        addstr("Taking Undercover Action:   What will ");
        addstr(cr->name);
        addstr(" focus on?");

        printcreatureinfo(cr);

        makedelimiter(8, 0);

        set_color(COLOR_WHITE, COLOR_BLACK, state == 'a');
        move(10, 1);
        addstr("A - Communication and Advocacy");

        set_color(COLOR_WHITE, COLOR_BLACK, state == 'b');
        move(11, 1);
        addstr("B - Espionage");

        set_color(COLOR_BLACK, COLOR_BLACK, 1); // Disabled
        move(12, 1);
        addstr("C - Sabotage and Direct Action");

        set_color(COLOR_WHITE, COLOR_BLACK, state == 'd');
        move(13, 1);
        addstr("D - Join the Active LCS");

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(20, 40);
        addstr("Enter - Confirm Selection");

        switch(state) {
        case 'a':
            set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_NONE);
            move(10, 40);
            addstr("1 - Lay Low");

            set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_SLEEPER_LIBERAL);
            move(11, 40);
            addstr("2 - Advocate Liberalism");

            /*set_color(COLOR_WHITE,COLOR_BLACK,cr->activity.type==ACTIVITY_SLEEPER_CONSERVATIVE);
            move(12,40);
            addstr("3 - Build Conservative Credibility");*/

            move(12, 40);

            if(subordinatesleft(*cr)) {
                set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_SLEEPER_RECRUIT);
                addstr("3 - Expand Sleeper Network");
            } else {
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

                if(cr->flag & CREATUREFLAG_BRAINWASHED)
                    addstr("3 - [Enlightened Can't Recruit]");
                else
                    addstr("3 - [Need More Juice to Recruit]");
            }

            break;

        case 'b':
            set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_SLEEPER_SPY);
            move(10, 40);
            addstr("1 - Uncover Secrets");

            set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_SLEEPER_EMBEZZLE);
            move(11, 40);
            addstr("2 - Embezzle Funds");

            set_color(COLOR_WHITE, COLOR_BLACK, cr->activity.type == ACTIVITY_SLEEPER_STEAL);
            move(12, 40);
            addstr("3 - Steal Equipment");
            break;

        case 'c':
            // Currently disabled
            /*set_color(COLOR_BLACK,COLOR_BLACK,1);
            move(10,40);
            addstr("1 - Fabricate Scandal");*/
            break;
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);

        switch(cr->activity.type) {
        case ACTIVITY_NONE:
            move(22, 3);
            addstr(cr->name);
            addstr(" will stay out of trouble.");
            break;

        case ACTIVITY_SLEEPER_LIBERAL:
            move(22, 3);
            addstr(cr->name);
            addstr(" will build support for Liberal causes.");
            break;

        /*case ACTIVITY_SLEEPER_CONSERVATIVE:
           move(22,3);
           addstr(cr->name);
           addstr(" will publicly condemn Liberals and the LCS, in exchange");
           move(23,3);
           addstr("for increased credibility with the good old boy network.");
           break;*/
        case ACTIVITY_SLEEPER_RECRUIT:
            if(subordinatesleft(*cr)) {
                move(22, 3);
                addstr(cr->name);
                addstr(" will try to recruit additional sleeper agents.");
            }

            break;

        case ACTIVITY_SLEEPER_SPY:
            move(22, 3);
            addstr(cr->name);
            addstr(" will snoop around for secrets and enemy plans.");
            break;

        case ACTIVITY_SLEEPER_EMBEZZLE:
            move(22, 3);
            addstr(cr->name);
            addstr(" will embezzle money for the LCS.");
            break;

        case ACTIVITY_SLEEPER_STEAL:
            move(22, 3);
            addstr(cr->name);
            addstr(" will steal equipment and send it to the Shelter.");
            break;

        case ACTIVITY_SLEEPER_SCANDAL:
            //move(22,3);
            //addstr(cr->name);
            //addstr(" will get creative and do something outrageous.");
            break;
        }

        refresh();
        int c = getch();
        translategetch(c);



        if(c >= 'a' && c <= 'z')
            state = c;

        if(c >= 'a' && c <= 'z' || c >= '1' && c <= '9') {
            choice = c;

            switch(state) {
            case 'a':
                switch(choice) {
                default:
                case '1':
                    cr->activity.type = ACTIVITY_NONE;
                    break;

                case '2':
                    cr->activity.type = ACTIVITY_SLEEPER_LIBERAL;
                    break;

                //case '3':cr->activity.type=ACTIVITY_SLEEPER_CONSERVATIVE;break;
                case '3':
                    if(subordinatesleft(*cr))
                        cr->activity.type = ACTIVITY_SLEEPER_RECRUIT;

                    break;
                }

                break;

            case 'b':
                switch(choice) {
                default:
                case '1':
                    cr->activity.type = ACTIVITY_SLEEPER_SPY;
                    break;

                case '2':
                    cr->activity.type = ACTIVITY_SLEEPER_EMBEZZLE;
                    break;

                case '3':
                    cr->activity.type = ACTIVITY_SLEEPER_STEAL;
                    break;
                }

                break;

            case 'c':
                switch(choice) {
                case '1':
                    break;//cr->activity.type=ACTIVITY_SLEEPER_SCANDAL;break;
                }

                break;
            }
        }

        if(state == 'd') {
            activityst oact = cr->activity;
            cr->activity.type = ACTIVITY_SLEEPER_JOINLCS;
        }

        if(c == 'x') {
            cr->activity.type = ACTIVITY_NONE;
            break;
        }

        if(c == 10)
            break;
    } while(1);
}