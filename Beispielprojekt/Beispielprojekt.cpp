#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <iostream>

using namespace std;


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


class GameWindow : public Gosu::Window
{
    Gosu::Font anzeige_score = { 30 };          //Font lässt variablen anzeigen 
    Gosu::Font anzeige_highscore = { 30 };      // 30 ist die größe
    Gosu::Font anzeige_gameover = { 50 };
    Gosu::Font anzeige_runde = { 30 };
    Player spieler;
    Gosu::Image Bildplayer;

    const double BalkenYBoden = 600;
    const double BalkenYDecke = 0;
    double maus_x = 0;
    double maus_y = 0;

    double score_zaehler = 0;
    double x_player = 100;
    double y_player = 300;
    double velocity = 3; //Bewegungs Geschwindigkeit
    int rot = 90;       //Rotation des Players
    double x_verschiebung = 400;
    double x1 = 500;
    double x2 = 600;
    double x3 = x1 + x_verschiebung;
    double x4 = x2 + x_verschiebung;
    double x5 = x3 + x_verschiebung;
    double x6 = x4 + x_verschiebung;
    double BalkenYMitteUnten = 300;

    double BalkenYMitteOben = 200;
    double y_verschiebung = 0;
    double y_verschiebung2 = 200;
    double y_verschiebung3 = -100;
    int speed_increase_at_score = 1;
    double score_increase = 0.016; //Weil die Update Methode 60mal pro Sekunde aufgerufen 
    int score = 0;
    double TimeDelayDeth = 0;        //Zeitdelay nach Tod
    int rundenzaehler = 0;
    bool gestorben = false;

public:

    GameWindow() : Window(850, 600), Bildplayer("biene.png")
    {
        set_caption("Flappy Biene");
    }

    void restart() {
        score_zaehler = 0;
        x_player = 100;
        y_player = 300;
        velocity = 3;
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

    void draw() override
    {
        string punkte = to_string(score);

        if (spieler.get_status()) {
            Bildplayer.draw_rot(x_player, y_player, 2, rot
            ,0.5,0.5,
               0.1,0.1
            );

            graphics().draw_quad(
                x1, BalkenYMitteUnten + y_verschiebung, Gosu::Color::WHITE,
                x2, BalkenYMitteUnten + y_verschiebung, Gosu::Color::WHITE,
                x2, BalkenYBoden, Gosu::Color::WHITE,
                x1, BalkenYBoden, Gosu::Color::WHITE,
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
            anzeige_score.draw_text("Score: " + punkte, 0, 0, 3, 1, 1, Gosu::Color::GREEN);



            if (gestorben) {          //Anzeige Menü Bildschirm

                velocity = 0;
                if (y_player < 590) {  //
                    y_player += 5; //Player fällt runter
                    rot = 180;
                }
                if (y_player >= 590) {
                    anzeige_gameover.draw_text("Game Over", 350, 275, 4, 1, 1, Gosu::Color::RED);
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

            graphics().draw_quad(0, 0, Gosu::Color::GREEN, 0, 600, Gosu::Color::GREEN, 850, 600, Gosu::Color::GREEN, 850, 0, Gosu::Color::GREEN, 4);
            graphics().draw_quad(375, 270, Gosu::Color::BLUE, 375, 330, Gosu::Color::BLUE, 475, 330, Gosu::Color::BLUE, 475, 270, Gosu::Color::BLUE, 4);
            Gosu::Font(30).draw_text("PLAY", 390, 285, 4, 1, 1, Gosu::Color::BLACK);


            string highscore_s = to_string(spieler.get_highscore());
            anzeige_highscore.draw_text("Highscore: " + highscore_s, 0, 0, 4, 1, 1, Gosu::Color::BLACK);

            string runde = to_string(rundenzaehler);
            if (rundenzaehler > 0)
                anzeige_runde.draw_text("Runde: " + runde, 0, 530, 4, 1, 1, Gosu::Color::BLACK);

            anzeige_score.draw_text("Score: " + punkte, 0, 570, 4, 1, 1, Gosu::Color::BLACK);
        }
    }

    // Wird 60x pro Sekunde aufgerufen
    void update() override
    {

        maus_x = input().mouse_x();            //Position Maus
        maus_y = input().mouse_y();

        if (spieler.get_status()) {

            if (!gestorben) {
                if (input().down(Gosu::Button::KB_RIGHT)) {               //Steuerung der Biene mit Pfeiltasten
                    x_player += velocity + 2;
                }
                if (input().down(Gosu::Button::KB_LEFT)) {
                    x_player -= velocity + 2;
                    rot = 270;
                }
                if (input().down(Gosu::Button::KB_UP)) {
                    y_player -= velocity + 2;
                    rot = 45;
                }
                if (input().down(Gosu::Button::KB_DOWN)) {
                    y_player += velocity + 2;
                    rot = 135;
                }
                if (!(input().down(Gosu::Button::KB_DOWN)) && !(input().down(Gosu::Button::KB_UP)) && !(input().down(Gosu::Button::KB_LEFT)))      //Rotation der Biene im Normalfall bei 90
                    rot = 90;
            }

            x1 -= velocity;           //bewegung der Balken
            x2 -= velocity;
            x3 -= velocity;
            x4 -= velocity;
            x5 -= velocity;
            x6 -= velocity;

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
                velocity++;
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

            if (x_player < 0 || x_player > 850 || y_player > 600 || y_player < 0)              //Abfrage, ob man außerhalb den Bildschirms geflogen ist->Game Over
                gestorben = true;

        }
        else {  //Menübildschierm 
            if (maus_x >= 375 && maus_x <= 475 && maus_y >= 270 && maus_y <= 330 && input().down(Gosu::Button::MS_LEFT)) {  //wenn im Menubild auf Play gedrückt wird, 
                restart();
                spieler.set_status(true);                                                                                                                                                                           //wird Player Status auf false zurückgesetzt und das Spiel somit gestartet                              
                rundenzaehler++;

            }
        }
    }
};


// C++ Hauptprogramm

int main()
{
    GameWindow GameWindow;

    GameWindow.show();


}

