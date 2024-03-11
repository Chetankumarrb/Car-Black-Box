

static char orgArr[4];
static char temp[4],i=0;
unsigned long int wait=0,a=0,c=3,ff=1,secC=0;


char *arr[9] = {"ON","GN","GR","G1","G2","G3","G4","G5","C "};
unsigned int index = 0,adc_val,col_flag=0,speed;
void Dashboard_diplay();
void passwd_display();
void main_menu();
char *menu[5] = {"VIEW_LOG    ","DOWNLOAD_LOG","CLEAR_LOG   ","SET TIME    ","CHANGE_PSWD "};
int m_index = 0,m_f=0,m_flag=0;

void __interrupt() isr();
void init_timer0();
unsigned int sec =120;


void  eeprom_storing();

int t_count = 0;
int e_flag=0;
int s_flag=0;



void View_Log();
void Download_Log();
void Clear_log();
void Set_time();
void Change_password();
int Clear_log_flag = 0;