#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

int main(int argc, char **argv)
{
    char user[100], groups[100], *group, **usergroups, *p;
    gid_t *groupids;
    struct passwd *pw;
    int i, ngroups;

    /*Get USER from stdin*/
    if(fgets(user, sizeof(user), stdin) == NULL)
    {
        exit(2);
    }

    /*Check input exists and strip newline*/
    if((p=strchr(user, '\n')) == NULL)
    {
        exit(4);
    }
    *p = '\0';

    /*Get GROUPS from stdin*/
    if(fgets(groups, sizeof(groups), stdin) == NULL)
    {
        exit(3);
    }

    /*Check input exists and strip newline*/
    if((p=strchr(groups, '\n')) == NULL)
    {
        exit(5);
    }
    *p = '\0';

    /*Get user's groups using getgrouplist*/
    ngroups = 100;
    groupids = malloc(ngroups * sizeof(gid_t));

    pw = getpwnam(user);
    if(pw == NULL)
    {
        exit(6);
    }
    getgrouplist(user, pw->pw_gid, groupids, &ngroups);

    /*Convert gids to names*/
    usergroups = malloc(ngroups * sizeof(char *));
    for(i = 0; i < ngroups; ++i)
    {
        usergroups[i] = (getgrgid(groupids[i]))->gr_name;
    }

    /*Compare user's groups to authorized groups*/
    group = strtok(groups, " ");
    while(group != NULL)
    {
        for(i = 0; i < ngroups; ++i)
        {
            if(strcmp(usergroups[i], group) == 0)
            {
                exit(0);
            }
        }
        group = strtok(NULL, " ");
    }
    exit(1);
}
