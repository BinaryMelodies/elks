#include <time.h>
#include <sys/time.h>

extern int _tz_is_set;
void __tm_conv(struct tm *tmbuf, const time_t *timep, time_t offset);

struct tm * localtime (const time_t * timep)
{
   static struct tm tmb;
   struct timezone tz;
   time_t offt;

   gettimeofday((void*)0, &tz);

   if (!_tz_is_set)
	tzset();
   tz.tz_minuteswest = timezone / 60;
   offt = -tz.tz_minuteswest*60L;

   /* tmb.tm_isdst = ? */
   __tm_conv(&tmb, timep, offt);

   return &tmb;
}
