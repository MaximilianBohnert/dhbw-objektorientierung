#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <iostream>
#include <vector>

using namespace std;

struct bonus {
    double x_pos_bonus;
    double y_pos_bonus;
};

class snake_teil {
public:
    double x_pos;
    double y_pos;
    int richtung;

    snake_teil(double x, double y, int r) : x_pos(x), y_pos(y), richtung(r) {}
};

class Player {
    string name;
    int highscore = 0;
    bool game_status = false; //Menübild oder nicht

public:

    string get_name() {
        return this->name;
    }
    void set_name(string name) {
        this->name = name;
    }
    int get_highscore() {
        return this->highscore;
    }
    void set_highscore(int score) {
        this->highscore = score;
    }
    bool get_status() { return this->game_status; }
    void set_status(bool status) { this->game_status = status; }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct SpaceAsteorid {
    const int AsteroidBreite = 30;
    const int AsteroidLaenge = 30;
    const int AsteroidPos = 2;
    const Gosu::Color FarbeAsteroid = Gosu::Color::GRAY;
    double AsteroidX;
    double AsteroidY;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameWindow : public Gosu::Window
{
    Gosu::Font anzeige_score_flappy_biene = { 30 };          //Font lässt variablen anzeigen 
    Gosu::Font anzeige_highscore_flappy_biene = { 30 };      // 30 ist die größe
    Gosu::Font anzeige_gameover_flappy_biene = { 50 };
    Gosu::Font anzeige_runde_flappy_biene = { 30 };
    Gosu::Font auswahlmenu = { 30 };
    Gosu::Font auswahl_flappy_biene = {30};
    Gosu::Font auswahl_snake = { 30 };
    Player spieler;
    Gosu::Image Bildplayer;
    Gosu::Image Hintergrund;

    const double BalkenYBoden = 600;
    const double BalkenYDecke = 0;
    double maus_x;
    double maus_y;

    double score_zaehler;
    double x_player ;
    double y_player;
    double velocity_flappy_biene ; //Bewegungs Geschwindigkeit
    int rot;       //Rotation des Players
    double x_verschiebung;
    double x1;
    double x2;
    double x3;
    double x4;
    double x5;
    double x6;
    double BalkenYMitteUnten;
    double BalkenYMitteOben;
    double y_verschiebung;
    double y_verschiebung2;
    double y_verschiebung3;
    int speed_increase_at_score;
    double score_increase; //Weil die Update Methode 60mal pro Sekunde aufgerufen 
    int score;
    double TimeDelayDeth;        //Zeitdelay nach Tod
    int rundenzaehler;
    bool gestorben;
    int spiel_auswahl = 0;

    Gosu::Font anzeige_score_snake = { 30 };
    Gosu::Font anzeige_restart_snake = { 30 };
    Gosu::Font anzeige_start_snake = { 30 };
    vector<bonus> bonus_liste;
    double velocity_snake = 3;
    bool rechts = true;
    bool links = false;
    bool oben = false;
    bool unten = false;
    int speed_increase_at = 1;
    double speed_increase = 1;
    int anzahl_bonus = 2;
    int bonus_increase_at = 2;
    double hitobx_increase = 0;
    int bonus_increase_abfrage = bonus_increase_at;
    bool pause = true;
    vector<snake_teil> snake;
    bool spielstatus = false;
    bool initial_start = true;

    //Space Spiel /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Gosu::Font anzeige_score_Space = { 30 };
    Gosu::Font anzeige_restart_Space = { 30 };
    Gosu::Font anzeige_start_Space = { 30 };
    const double SpaceBeeX = 50;
    double  SpaceBeeY =400;
    int SpaceScore;
    int rotationSpace = 270;
    vector<SpaceAsteorid> Asteroiden;
    double SpaceSpeed = 3;
    bool SpaceSpielen = false;
    int AstAnzahl = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

public:

    GameWindow() : Window(1000, 600), Bildplayer("biene.png"),Hintergrund("pixelhimmel.png")
    {
        set_caption("Flappy Biene");
    }

    void restart_flappy_bird() {
        score_zaehler = 0;
        x_player = 100;
        y_player = 300;
        velocity_flappy_biene = 3;
        rot = 90;
        x_verschiebung = 400;
        x1 = 500;
        x2 = 600;
        x3 = x1 + x_verschiebung;
        x4 = x2 + x_verschiebung;
        x5 = x3 + x_verschiebung;
        x6 = x4 + x_verschiebung;
        BalkenYMitteUnten = 300;
        BalkenYMitteOben = 200;
        y_verschiebung = 0;
        y_verschiebung2 = 200;
        y_verschiebung3 = -100;
        speed_increase_at_score = 1;
        score_increase = 0.016;
        score = 0;
        TimeDelayDeth = 0;        //Zeitdelay nach Tod
        rundenzaehler = 0;
        gestorben = false;
    }

    void restart_snake() {
        bonus_liste.clear();
        snake.clear();
        rechts = true; links = false; oben = false; unten = false;
        speed_increase_at = 1;
        speed_increase = 1;
        anzahl_bonus = 2;
        hitobx_increase = 0;
        velocity_snake = 3;
        bonus_increase_at = 2;
        bonus_increase_abfrage = bonus_increase_at;
        spielstatus = true;
        pause = false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void restart_space() {
        Asteroiden.clear();
        
        SpaceBeeY = 400;
        SpaceScore = 0;
        gestorben = false;
        SpaceSpielen = true;
        AstAnzahl = 0;

    }
    void erstelleAsteroid(vector<SpaceAsteorid>& AstVect) {
        double y = Gosu::random(0, 800);
        SpaceAsteorid ast;
        ast.AsteroidX = 1000;
        ast.AsteroidY = y;
        AstVect.push_back(ast);
    }/*
    void AstInRange(vector<SpaceAsteorid>& AstVect) {
        for (int laenge = 0; laenge <= AstVect.size()-1;laenge++) {
            if (AstVect.at(laenge).AsteroidX <= (0-(AstVect.at(laenge).AsteroidBreite / 2))) {
                erase(AstVect, AstVect[laenge]);   //Löscht Objekte die auserhalb des Feldes sind
            }
        }
    }*/
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void draw() override
    {
        if (spiel_auswahl == 0) {

            graphics().draw_quad(150, 270, Gosu::Color::BLUE, 150, 330, Gosu::Color::BLUE, 350, 330, Gosu::Color::BLUE, 350, 270, Gosu::Color::BLUE, 1);
            graphics().draw_quad(670, 270, Gosu::Color::BLUE, 670, 330, Gosu::Color::BLUE, 830, 330, Gosu::Color::BLUE, 830, 270, Gosu::Color::BLUE, 1);
            graphics().draw_quad(430, 270, Gosu::Color::BLUE, 430, 330, Gosu::Color::BLUE, 600, 330, Gosu::Color::BLUE, 600, 270, Gosu::Color::BLUE, 1);

            auswahl_flappy_biene.draw_text_rel("Flappy Biene", 250, 300, 1, 0.5, 0.5, 1, 1, Gosu::Color::BLACK);
            auswahl_snake.draw_text_rel("Snake", 750, 300, 1, 0.5, 0.5, 1, 1, Gosu::Color::BLACK);
            auswahl_flappy_biene.draw_text_rel("Space", 515, 300, 1, 0.5, 0.5, 1, 1, Gosu::Color::BLACK);
        }

        if (spiel_auswahl == 1) {
            string punkte = to_string(score);

            if (spieler.get_status()) {
                Bildplayer.draw_rot(x_player, y_player, 2, rot
                    , 0.5, 0.5,
                    0.1, 0.1
                );
                Hintergrund.draw(0, 0, 0, 2, 2);

                graphics().draw_quad(
                    x1, BalkenYMitteUnten + y_verschiebung, Gosu::Color::GREEN,
                    x2, BalkenYMitteUnten + y_verschiebung, Gosu::Color::GREEN,
                    x2, BalkenYBoden, Gosu::Color::GREEN,
                    x1, BalkenYBoden, Gosu::Color::GREEN,
                    1
                );

                graphics().draw_quad(
                    x1, BalkenYDecke, Gosu::Color::WHITE,
                    x2, BalkenYDecke, Gosu::Color::WHITE,
                    x2, BalkenYMitteOben + y_verschiebung, Gosu::Color::WHITE,
                    x1, BalkenYMitteOben + y_verschiebung, Gosu::Color::WHITE,
                    1
                );

                graphics().draw_quad(
                    x3, BalkenYMitteUnten + y_verschiebung2, Gosu::Color::WHITE,
                    x4, BalkenYMitteUnten + y_verschiebung2, Gosu::Color::WHITE,
                    x4, BalkenYBoden, Gosu::Color::WHITE,
                    x3, BalkenYBoden, Gosu::Color::WHITE,
                    1
                );

                graphics().draw_quad(
                    x3, BalkenYDecke, Gosu::Color::WHITE,
                    x4, BalkenYDecke, Gosu::Color::WHITE,
                    x4, BalkenYMitteOben + y_verschiebung2, Gosu::Color::WHITE,
                    x3, BalkenYMitteOben + y_verschiebung2, Gosu::Color::WHITE,
                    1
                );

                graphics().draw_quad(
                    x5, BalkenYMitteUnten + y_verschiebung3, Gosu::Color::WHITE,
                    x6, BalkenYMitteUnten + y_verschiebung3, Gosu::Color::WHITE,
                    x6, BalkenYBoden, Gosu::Color::WHITE,
                    x5, BalkenYBoden, Gosu::Color::WHITE,
                    1
                );

                graphics().draw_quad(
                    x5, BalkenYDecke, Gosu::Color::WHITE,
                    x6, BalkenYDecke, Gosu::Color::WHITE,
                    x5, BalkenYMitteOben + y_verschiebung3, Gosu::Color::WHITE,
                    x6, BalkenYMitteOben + y_verschiebung3, Gosu::Color::WHITE,
                    1
                );


                //Score Anzeige während des Spiels
                anzeige_score_flappy_biene.draw_text("Score: " + punkte, 0, 0, 3, 1, 1, Gosu::Color::GREEN);



                if (gestorben) {          //Anzeige Menü Bildschirm

                    velocity_flappy_biene = 0;
                    if (y_player < 590) {  //
                        y_player += 5; //Player fällt runter
                        rot = 180;
                    }
                    if (y_player >= 590) {
                        anzeige_gameover_flappy_biene.draw_text_rel("Game Over", 500, 300, 4, 0.5, 0.5, 1, 1, Gosu::Color::RED);
                        rot = 90;
                        TimeDelayDeth += 0.08;
                    }

                }
                if (TimeDelayDeth >= 5)
                    spieler.set_status(false);
            }
            if (!spieler.get_status()) {

                if (spieler.get_highscore() < score)
                    spieler.set_highscore(score);

                graphics().draw_quad(0, 0, Gosu::Color::GREEN, 0, 600, Gosu::Color::GREEN, 1000, 600, Gosu::Color::GREEN, 1000, 0, Gosu::Color::GREEN, 0);
                graphics().draw_quad(450, 270, Gosu::Color::BLUE, 450, 330, Gosu::Color::BLUE, 550, 330, Gosu::Color::BLUE, 550, 270, Gosu::Color::BLUE, 1);
                Gosu::Font(30).draw_text_rel("PLAY", 500, 300, 2, 0.5, 0.5, 1, 1, Gosu::Color::BLACK);

                graphics().draw_quad(700, 40, Gosu::Color::BLUE, 700, 100, Gosu::Color::BLUE, 960, 100, Gosu::Color::BLUE, 960, 40, Gosu::Color::BLUE, 1);
                auswahlmenu.draw_text_rel("Auswahlbildschirm", 830, 70, 2, 0.5, 0.5,1, 1, Gosu::Color::BLACK);


                string highscore_s = to_string(spieler.get_highscore());
                anzeige_highscore_flappy_biene.draw_text("Highscore: " + highscore_s, 0, 0, 4, 1, 1, Gosu::Color::BLACK);

                string runde = to_string(rundenzaehler);
                if (rundenzaehler > 0)
                    anzeige_runde_flappy_biene.draw_text("Runde: " + runde, 0, 530, 2, 1, 1, Gosu::Color::BLACK);

                anzeige_score_flappy_biene.draw_text_rel("Score: " + punkte, 0, 600, 2, 0, 1, 1, 1, Gosu::Color::BLACK);
            }
        }       

        if (spiel_auswahl == 2) {

            string score = to_string(snake.size());
            anzeige_score_snake.draw_text_rel("Score: " + score, 0, 0, 2, 0, 0, 1, 1, Gosu::Color::GREEN);

            if (initial_start) {
                anzeige_start_snake.draw_text_rel("Start", 500, 300, 2, 0.5, 0.5, 2, 2, Gosu::Color::RED);
            }

            if (!spielstatus || initial_start) {
                graphics().draw_quad(700, 40, Gosu::Color::BLUE, 700, 100, Gosu::Color::BLUE, 960, 100, Gosu::Color::BLUE, 960, 40, Gosu::Color::BLUE, 1);
                auswahlmenu.draw_text_rel("Auswahlbildschirm", 830, 70, 2, 0.5, 0.5, 1, 1, Gosu::Color::BLACK);
            }

            if (!spielstatus && !initial_start) {
                pause = true;
                anzeige_restart_snake.draw_text_rel("Restart", 500, 300, 2, 0.5, 0.5, 2, 2, Gosu::Color::RED);
            }

            if (!pause && spielstatus) {
                graphics().draw_quad(980, 20, Gosu::Color::WHITE,
                    970, 20, Gosu::Color::WHITE,
                    980, 50, Gosu::Color::WHITE,
                    970, 50, Gosu::Color::WHITE, 1);
                graphics().draw_quad(965, 20, Gosu::Color::WHITE,
                    955, 20, Gosu::Color::WHITE,
                    965, 50, Gosu::Color::WHITE,
                    955, 50, Gosu::Color::WHITE, 1);
            }
            else if (pause && spielstatus) {
                graphics().draw_triangle(925, 20, Gosu::Color::WHITE,
                    925, 50, Gosu::Color::WHITE,
                    950, 35, Gosu::Color::WHITE, 1);

            }
            for (auto it = bonus_liste.begin(); it != bonus_liste.end(); it++) {
                graphics().draw_quad(it->x_pos_bonus - 5, it->y_pos_bonus - 5, Gosu::Color::GREEN,
                    it->x_pos_bonus + 5, it->y_pos_bonus - 5, Gosu::Color::GREEN,
                    it->x_pos_bonus + 5, it->y_pos_bonus + 5, Gosu::Color::GREEN,
                    it->x_pos_bonus - 5, it->y_pos_bonus + 5, Gosu::Color::GREEN, 1);
            }

            for (auto it = snake.begin(); it != snake.end(); it++) {
                if (it == snake.begin()) {
                    graphics().draw_quad(it->x_pos - 10, it->y_pos - 10, Gosu::Color::BLUE,
                        it->x_pos + 10, it->y_pos - 10, Gosu::Color::BLUE,
                        it->x_pos + 10, it->y_pos + 10, Gosu::Color::BLUE,
                        it->x_pos - 10, it->y_pos + 10, Gosu::Color::BLUE, 1);

                    graphics().draw_quad(it->x_pos - 5, it->y_pos - 5, Gosu::Color::RED,
                        it->x_pos + 5, it->y_pos - 5, Gosu::Color::RED,
                        it->x_pos + 5, it->y_pos + 5, Gosu::Color::RED,
                        it->x_pos - 5, it->y_pos + 5, Gosu::Color::RED, 1);
                }
                else {
                    graphics().draw_quad(it->x_pos - 10, it->y_pos - 10, Gosu::Color::BLUE,
                        it->x_pos + 10, it->y_pos - 10, Gosu::Color::BLUE,
                        it->x_pos + 10, it->y_pos + 10, Gosu::Color::BLUE,
                        it->x_pos - 10, it->y_pos + 10, Gosu::Color::BLUE, 1);
                }
            }

        }   
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (spiel_auswahl == 3) {
            if (SpaceSpielen) {
                Bildplayer.draw_rot(SpaceBeeX, SpaceBeeY, 2, rotationSpace
                    , 0.5, 0.5,
                    0.1, 0.1
                );

                for (SpaceAsteorid ast : Asteroiden) {
                    graphics().draw_rect(ast.AsteroidX, ast.AsteroidY, ast.AsteroidLaenge, ast.AsteroidBreite, ast.FarbeAsteroid, ast.AsteroidPos);
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    // Wird 60x pro Sekunde aufgerufen
    void update() override
    {

        maus_x = input().mouse_x();            //Position Maus
        maus_y = input().mouse_y();

        if (spiel_auswahl == 0) {
            if (maus_x >= 150 && maus_x <= 350 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT)) {
                spiel_auswahl = 1;
            }
            else if (maus_x >= 670 && maus_x <= 830 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT)) {
                spiel_auswahl = 2;
               // spielstatus = false;
            }
            else if (maus_x >= 430 && maus_x <= 600 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT)) {
                spiel_auswahl = 3;
                SpaceSpielen = true;
               
            }
        }

        if (spiel_auswahl == 1) {
            if (spieler.get_status()) {

                if (!gestorben) {
                    if (input().down(Gosu::Button::KB_RIGHT)) {               //Steuerung der Biene mit Pfeiltasten
                        x_player += velocity_flappy_biene + 2;
                    }
                    if (input().down(Gosu::Button::KB_LEFT)) {
                        x_player -= velocity_flappy_biene + 2;
                        rot = 270;
                    }
                    if (input().down(Gosu::Button::KB_UP)) {
                        y_player -= velocity_flappy_biene + 2;
                        rot = 45;
                    }
                    if (input().down(Gosu::Button::KB_DOWN)) {
                        y_player += velocity_flappy_biene + 2;
                        rot = 135;
                    }
                    if (!(input().down(Gosu::Button::KB_DOWN)) && !(input().down(Gosu::Button::KB_UP)) && !(input().down(Gosu::Button::KB_LEFT)))      //Rotation der Biene im Normalfall bei 90
                        rot = 90;
                }

                x1 -= velocity_flappy_biene;           //bewegung der Balken
                x2 -= velocity_flappy_biene;
                x3 -= velocity_flappy_biene;
                x4 -= velocity_flappy_biene;
                x5 -= velocity_flappy_biene;
                x6 -= velocity_flappy_biene;

                if (x2 < 0) {             //neu spawnen der Balken mit random Werten für Loch, das Biene durchqueren muss
                    x1 = x5 + x_verschiebung;
                    x2 = x6 + x_verschiebung;
                    y_verschiebung = Gosu::random(-200, 300);
                }

                if (x4 < 0) {
                    x3 = x1 + x_verschiebung;
                    x4 = x2 + x_verschiebung;
                    y_verschiebung2 = Gosu::random(-200, 300);
                }

                if (x6 < 0) {
                    x5 = x3 + x_verschiebung;
                    x6 = x4 + x_verschiebung;
                    y_verschiebung3 = Gosu::random(-200, 300);
                }
                if (!gestorben) {
                    score_zaehler += score_increase;             //Scorezähler->Score ist int, score_zaehler ist double und wird immer um score_increade (= 1/60) erhöht->wenn score_zaehler 1 ist, dann wird score um 1 erhöht
                }
                if (score_zaehler > 1) {
                    score += 1;
                    score_zaehler = 0;
                }
                if (score == speed_increase_at_score) {             //wenn bestimmter Score erreicht wird, steigt Geschwindigkeit, Score_increase -> passiert alle 50 Score Punkte
                    velocity_flappy_biene++;
                    speed_increase_at_score += speed_increase_at_score;
                    score_increase += 0.01;
                }


                if (x_player >= x1 && x_player <= x2 && ((y_player >= (BalkenYMitteUnten + y_verschiebung) && y_player <= BalkenYBoden) || (y_player >= BalkenYDecke && y_player <= BalkenYMitteOben + y_verschiebung))) { //Abfrage, ob man gegen Hinderniss geflogen ist -> Game Over
                    gestorben = true;

                }
                if (x_player >= x3 && x_player <= x4 && ((y_player >= BalkenYMitteUnten + y_verschiebung2 && y_player <= BalkenYBoden) || (y_player >= BalkenYDecke && y_player <= BalkenYMitteOben + y_verschiebung2))) {
                    gestorben = true;

                }
                if (x_player >= x5 && x_player <= x6 && ((y_player >= BalkenYMitteUnten + y_verschiebung3 && y_player <= BalkenYBoden) || (y_player >= BalkenYDecke && y_player <= BalkenYMitteOben + y_verschiebung3))) {
                    gestorben = true;

                }

                if (x_player < 0 || x_player > 11000 || y_player > 600 || y_player < 0)              //Abfrage, ob man außerhalb den Bildschirms geflogen ist->Game Over
                    gestorben = true;

            }
            else {  //Menübildschierm 
                if (maus_x >= 450 && maus_x <= 550 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT)) {  //wenn im Menubild auf Play gedrückt wird, 
                    restart_flappy_bird();
                    spieler.set_status(true);                                                                                                                                                                           //wird Player Status auf false zurückgesetzt und das Spiel somit gestartet                              
                    rundenzaehler++;

                }
                if (maus_x >= 700 && maus_x <= 960 && maus_y >= 40 && maus_y <= 100 && input().down(Gosu::Button::MS_LEFT)) {
                    spiel_auswahl = 0;
                }
            }
        }       

        if (spiel_auswahl == 2) {

            if (snake.size() == 0) {
                snake.push_back({ 200, 300, 1 });
            }

            if (speed_increase_at == snake.size()) {
                velocity_snake += speed_increase;
                speed_increase_at += speed_increase_at;
                hitobx_increase += 1;
            }

            if (maus_x >= 700 && maus_x <= 960 && maus_y >= 40 && maus_y <= 100 && input().down(Gosu::Button::MS_LEFT)) {
                spiel_auswahl = 0;
            }

            if (maus_x >= 450 && maus_x <= 550 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT) && initial_start) {		//start Button gedrückt
                initial_start = false;
                restart_snake();
                return;
            }

            if (maus_x >= 410 && maus_x <= 590 && maus_y >= 270 && maus_y <= 330 && !spielstatus && input().down(Gosu::Button::MS_LEFT) && !initial_start) {		//reset Button gedrückt
                restart_snake();
                return;
            }

            if ((maus_x >= 955 && maus_x <= 980 && maus_y >= 20 && maus_y <= 50 && input().down(Gosu::Button::MS_LEFT)) || input().down(Gosu::Button::KB_P)) {
                pause = true;
            }
            if ((maus_x >= 925 && maus_x <= 950 && maus_y >= 20 && maus_y <= 50 && input().down(Gosu::Button::MS_LEFT) && pause && spielstatus) || input().down(Gosu::Button::KB_C))
                pause = false;


            if (input().down(Gosu::Button::KB_RIGHT) || rechts) {
                if (links && snake.size() > 1) {
                    spielstatus = false;
                    return;
                }
                rechts = true; links = false; oben = false; unten = false;
                snake.at(0).richtung = 1;

            }
            if (input().down(Gosu::Button::KB_LEFT) || links) {
                if (rechts && snake.size() > 1) {
                    spielstatus = false;
                    return;
                }
                rechts = false; links = true; oben = false; unten = false;
                snake.at(0).richtung = 2;


            }
            if (input().down(Gosu::Button::KB_UP) || oben) {
                if (unten && snake.size() > 1) {
                    spielstatus = false;
                    return;
                }
                rechts = false; links = false; oben = true; unten = false;
                snake.at(0).richtung = 3;
            }
            if (input().down(Gosu::Button::KB_DOWN) || unten) {
                if (oben && snake.size() > 1) {
                    spielstatus = false;
                    return;
                }
                rechts = false; links = false; oben = false; unten = true;
                snake.at(0).richtung = 4;

            }
            if (!pause) {
                if (rechts)
                    snake.at(0).x_pos += velocity_snake;
                if (links)
                    snake.at(0).x_pos -= velocity_snake;
                if (oben)
                    snake.at(0).y_pos -= velocity_snake;
                if (unten)
                    snake.at(0).y_pos += velocity_snake;
            }

            auto it_außen_vorn = snake.begin();
            auto it_außen_hinten = snake.begin();
            it_außen_hinten++;
            if (!pause) {
                for (it_außen_hinten; it_außen_hinten != snake.end(); it_außen_hinten++) {

                    if (it_außen_vorn->richtung == 1 || it_außen_vorn->richtung == 2) {
                        if (it_außen_hinten->y_pos < it_außen_vorn->y_pos - (1 + hitobx_increase))
                            it_außen_hinten->y_pos += velocity_snake;
                        else if (it_außen_hinten->y_pos > it_außen_vorn->y_pos + 1 + hitobx_increase)
                            it_außen_hinten->y_pos -= velocity_snake;
                        else {
                            if (it_außen_hinten->y_pos != it_außen_vorn->y_pos) {
                                it_außen_hinten->y_pos = it_außen_vorn->y_pos;
                            }
                            if (it_außen_vorn->richtung == 1) {
                                if (it_außen_hinten->x_pos != it_außen_vorn->x_pos - 25) {
                                    it_außen_hinten->x_pos = it_außen_vorn->x_pos - 25;
                                }
                                else if (it_außen_hinten->x_pos < it_außen_vorn->x_pos)
                                    it_außen_hinten->x_pos += velocity_snake;
                                it_außen_hinten->richtung = 1;
                            }
                            if (it_außen_vorn->richtung == 2) {
                                if (it_außen_hinten->x_pos != it_außen_vorn->x_pos + 25) {
                                    it_außen_hinten->x_pos = it_außen_vorn->x_pos + 25;
                                }
                                else if (it_außen_hinten->x_pos > it_außen_vorn->x_pos)
                                    it_außen_hinten->x_pos -= velocity_snake;
                                it_außen_hinten->richtung = 2;
                            }
                        }

                    }
                    if (it_außen_vorn->richtung == 3 || it_außen_vorn->richtung == 4) {
                        if (it_außen_hinten->x_pos < it_außen_vorn->x_pos - (1 + hitobx_increase))
                            it_außen_hinten->x_pos += velocity_snake;
                        else if (it_außen_hinten->x_pos > it_außen_vorn->x_pos + 1 + hitobx_increase)
                            it_außen_hinten->x_pos -= velocity_snake;
                        else {
                            if (it_außen_hinten->x_pos != it_außen_vorn->x_pos) {
                                it_außen_hinten->x_pos = it_außen_vorn->x_pos;
                            }
                            if (it_außen_vorn->richtung == 3) {
                                if (it_außen_hinten->y_pos != it_außen_vorn->y_pos + 25) {
                                    it_außen_hinten->y_pos = it_außen_vorn->y_pos + 25;
                                }
                                else if (it_außen_hinten->y_pos > it_außen_vorn->y_pos)
                                    it_außen_hinten->y_pos -= velocity_snake;
                                it_außen_hinten->richtung = 3;
                            }
                            if (it_außen_vorn->richtung == 4) {
                                if (it_außen_hinten->y_pos != it_außen_vorn->y_pos - 25) {
                                    it_außen_hinten->y_pos = it_außen_vorn->y_pos - 25;
                                }
                                else if (it_außen_hinten->y_pos < it_außen_vorn->y_pos)
                                    it_außen_hinten->y_pos += velocity_snake;
                                it_außen_hinten->richtung = 4;
                            }
                        }

                    }
                    if ((snake.at(0).x_pos < it_außen_hinten->x_pos + 10 && snake.at(0).x_pos > it_außen_hinten->x_pos - 10) && (snake.at(0).y_pos < it_außen_hinten->y_pos + 10 && snake.at(0).y_pos > it_außen_hinten->y_pos - 10)) {
                        spielstatus = false;
                        return;
                    }

                    it_außen_vorn++;
                }
            }

            if (snake.at(0).x_pos + 10 > 1000 || snake.at(0).x_pos - 10 < 0 || snake.at(0).y_pos + 10 > 600 || snake.at(0).y_pos - 10 < 0) {
                spielstatus = false;
                return;
            }



            if (snake.size() == bonus_increase_abfrage && anzahl_bonus < 5) {
                anzahl_bonus++;
                bonus_increase_abfrage += bonus_increase_at;
            }

            if (bonus_liste.size() < anzahl_bonus) {
                bonus_liste.push_back({ Gosu::random(100, 900), Gosu::random(100, 500) });
            }

            for (auto it = bonus_liste.begin(); it != bonus_liste.end(); it++) {
                if (it->x_pos_bonus >= snake.at(0).x_pos - 10 && it->x_pos_bonus <= (snake.at(0).x_pos + 10) && it->y_pos_bonus >= snake.at(0).y_pos - 10 && it->y_pos_bonus <= (snake.at(0).y_pos + 10)) {		//x_pos_bonus >= snake.at(0).x_pos && x_pos_bonus <= (snake.at(0).x_pos + 20) && y_pos_bonus >= snake.at(0).y_pos && y_pos_bonus <= (snake.at(0).y_pos +20)
                    if (rechts)
                        snake.push_back({ snake.at(snake.size() - 1).x_pos - 25, snake.at(snake.size() - 1).y_pos, 1 });
                    if (links)
                        snake.push_back({ snake.at(snake.size() - 1).x_pos + 25, snake.at(snake.size() - 1).y_pos, 2 });
                    if (oben)
                        snake.push_back({ snake.at(snake.size() - 1).x_pos, snake.at(snake.size() - 1).y_pos + 25, 3 });
                    if (unten)
                        snake.push_back({ snake.at(snake.size() - 1).x_pos, snake.at(snake.size() - 1).y_pos - 25, 4 });
                    it->x_pos_bonus = Gosu::random(100, 900);
                    it->y_pos_bonus = Gosu::random(100, 500);
                }
            }

        }      
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (spiel_auswahl == 3) {
            if (!gestorben) {
                AstAnzahl--;
                if (input().down(Gosu::Button::KB_UP)) {
                    SpaceBeeY -= (SpaceSpeed + 2);
                }
                else if (input().down(Gosu::Button::KB_DOWN)) {
                    SpaceBeeY += SpaceSpeed + 2;
                }
                if (AstAnzahl <= 1) {
                    AstAnzahl = 30;
                    erstelleAsteroid(Asteroiden);
                }

            }
            for (SpaceAsteorid& ast : Asteroiden) {
                ast.AsteroidX = ast.AsteroidX - SpaceSpeed;
                if ((SpaceBeeX <= (ast.AsteroidX + (ast.AsteroidBreite / 2)) && SpaceBeeX >= (ast.AsteroidX - (ast.AsteroidBreite / 2))) && 
                    (SpaceBeeY <= (ast.AsteroidY - (ast.AsteroidLaenge / 2)) && SpaceBeeY >= (ast.AsteroidY + (ast.AsteroidLaenge / 2)))) 
                {
                    gestorben = true;
                    break;
                }
            }

            if (SpaceBeeY > 600 || SpaceBeeY< 0)              //Abfrage, ob man außerhalb den Bildschirms geflogen ist->Game Over
                gestorben = true;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
};


// C++ Hauptprogramm

int main()
{
    GameWindow GameWindow;

    GameWindow.show();


}

