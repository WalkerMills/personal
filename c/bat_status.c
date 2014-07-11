#include <stdio.h>

#include <linux/power_supply.h>

#define BAT "BAT0"

int main() {
    char *name = BAT;
    int result = 0;

    struct power_supply *psy = power_supply_get_by_name(name);
    union power_supply_propval property;

    
    result = psy->get_property(psy, POWER_SUPPLY_PROP_CAPACITY, &property);
    if ( !result ) {
        printf("(%d%): ", property.intval);
    }

    result = psy->get_property(psy, POWER_SUPPLY_PROP_STATUS, &property);
    if ( !result ) {
        
    }

}
