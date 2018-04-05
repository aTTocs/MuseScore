#include "bagpipepanel.h"
#include "ui_bagpipepanel.h"

BagpipePanel::BagpipePanel(QWidget *parent) :
      QDockWidget(parent),
      ui(new Ui::BagpipePanel)
      {
      ui->setupUi(this);
      }

BagpipePanel::~BagpipePanel()
      {
      delete ui;
      }
