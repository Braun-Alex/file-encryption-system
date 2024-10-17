#include "app.h"
#include "file-picker.h"

int main(int argc, char** argv) {
    auto ui = App::create();

    ui->on_pick_files([&]{
        ui->set_files(pick_files());
    });

    ui->run();
    return 0;
}
