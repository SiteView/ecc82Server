module site
{
       sequence<string> lists;
       
       interface HttpClinetBean
       {
             string getHTML(string URL);
             string login(lists list, string URL);
             void init(string url,int port,string http);
             void deluetInit();
       };


       interface HttpParser
       {
              lists getForm(string HTML);
              lists getFormInput(string HTML, int number);
              lists getFormInputValue(string HTML, int number);
              string getFormName(string HTML);
              lists getLink(string HTML);
       };

};
