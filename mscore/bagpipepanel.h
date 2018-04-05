#ifndef BAGPIPEPANEL_H
#define BAGPIPEPANEL_H

#include <QDockWidget>

namespace Ui {
class BagpipePanel;
}

class BagpipePanel : public QDockWidget
      {
      Q_OBJECT

public:
      explicit BagpipePanel(QWidget *parent = 0);
      ~BagpipePanel();

private:
      Ui::BagpipePanel *ui;
      };

#endif // BAGPIPEPANEL_H
