/*
 * menu.h
 *
 * Created: 2014-11-11 11:39:25
 *  Author: Marcin
 */ 


#ifndef MENU_H_
#define MENU_H_

#define LCD_COLLS 16
#define LCD_ROWS 2

//////////////////////////////////////////////////////////////////////////
//DEKLARACJE FUNKCJI OBS£UGI MENU
//////////////////////////////////////////////////////////////////////////
const struct _menuitem *GetMenuItem(uint8_t index);
void Menu_Back(void);
void Menu_Click();
void Menu_SelectPrev(void);
void Menu_SelectNext(void);
void Menu_Show(void);



#endif /* MENU_H_ */