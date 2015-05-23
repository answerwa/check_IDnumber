#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<regex.h>
#include<time.h>

#define PATTERN "[0-9]{17}([0-9]|x)"
int offset_store;
void numbermatch(const char *number)
{
	int iNumber[] = {7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
	char lastChar[] = {'1','0','X','9','8','7','6','5','4','3','2'}; 

	char area[] = {
		0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,1,1,0,0,0,0,
		0,1,1,1,0,0,0,0,0,0,
		0,1,1,1,1,1,1,1,0,0,
		0,1,1,1,1,1,1,0,0,0,
		1,1,1,1,1,0,0,0,0,0,
		0,1,1,1,1,1,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,
		0,1,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	regex_t reg;
	const size_t nmatch = 1;
	regmatch_t pmatch[1];

	int i, j, err, status;
	char errbuf[1024];
	int sum = 0;
	
	int year, month, day;

	int area_number;
#ifdef YEAR
	time_t now;
	struct tm *tm;
#endif

	status = regcomp(&reg, PATTERN, REG_EXTENDED|REG_ICASE);
	if(status < 0) {
		regerror(err, &reg, errbuf, sizeof(errbuf));
		printf("err:%s\n", errbuf);
	}

	status = regexec(&reg, number, nmatch, pmatch, 0);
	if (status == REG_NOMATCH) {
		printf("Match Over\n");
	} else if(status == 0) {
		// check area number
		area_number = (number[pmatch[0].rm_so]-48)* 10 + (number[pmatch[0].rm_so+1]-48);
		if(area[area_number] == 0) {
			printf("area is %d, wrong! offset is %d\n", area_number, offset_store + pmatch[0].rm_so);
			offset_store += pmatch[0].rm_so+1;
			numbermatch(number+pmatch[0].rm_so+1);
		} else {
#ifdef YEAR
			//check year number
			time(&now);
			tm = localtime(&now);
	
			year = (number[pmatch[0].rm_so+6]-48)*1000 + (number[pmatch[0].rm_so+7]-48)*100 + (number[pmatch[0].rm_so+8]-48)*10 + (number[pmatch[0].rm_so+9]-48);
			if(year > (tm->tm_year+1900) || tm->tm_year+1900-year > 150) {
				printf("birth year is %d, wrong! offset is %d\n", year, offset_store + pmatch[0].rm_so);
				offset_store += pmatch[0].rm_so+1;
				numbermatch(number+pmatch[0].rm_so+1);
			} else {
#endif
				// check month number
				month = (number[pmatch[0].rm_so+10]-48)*10 + (number[pmatch[0].rm_so+11]-48);
				if(month > 12 || month == 0) {
					printf("month is %d, wrong! offset is %d\n", month, offset_store + pmatch[0].rm_so);
					offset_store += pmatch[0].rm_so+1;
					numbermatch(number+pmatch[0].rm_so+1);
				} else {
					//check day number
					day = (number[pmatch[0].rm_so+12]-48)*10 + (number[pmatch[0].rm_so+13]-48);
					if(day > 31 || day == 0) {
						printf("day is %d, wrong! offset is %d\n", day, offset_store + pmatch[0].rm_so);
						offset_store += pmatch[0].rm_so+1;
						numbermatch(number+pmatch[0].rm_so+1);
					} else {
						// check last number
						for(i=pmatch[0].rm_so, j=0; j<17; ++i,++j)
							sum += (number[i] - 48) * iNumber[j];
						if(lastChar[sum%11] != number[i]) {
							printf("last number should be %c! offset is %d\n", lastChar[sum%11], offset_store + pmatch[0].rm_so);
							offset_store += pmatch[0].rm_so+1;
							numbermatch(number+pmatch[0].rm_so+1);
						} else {
							// confirm ID number
							printf("***this is a ID number! offset is %d***\n", offset_store + pmatch[0].rm_so);
							offset_store += pmatch[0].rm_eo;
							numbermatch(number+pmatch[0].rm_eo);
						}
					}
				}
			}
#ifdef YEAR
		}
#endif
	} else {
		regerror(err, &reg, errbuf, sizeof(errbuf));
		printf("err:%s\n", errbuf);
	}
	regfree(&reg);
}

int main()
{
	char number[128];
	printf("input number:");
	gets(number);
	numbermatch(number);
	return 0;
}
