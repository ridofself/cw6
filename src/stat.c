/* stat.c */

struct stat
{
	float grain; /* weight minus fat and water; measures strength */
	float water; /* pounds of water in body */
	float fat; /* pounds of fat in body */

	float fat_used; /* based on minutes worked without rest */
	float fat_use_max;
};

float stat_s_curve(float percent) /* argument between 1.0 and 0.0 */
{
	/* gradual initial decline, rapid decline, gradual final decline */
	return (3 * (percent*percent)) - (2 * (percent*percent*percent));
}

float stat_weight(struct stat* stat)
{
	/* total pounds of body weight minus tare weight (clothes, etc) */
	return stat->grain + stat->water + stat->fat;
}

float stat_hydration(struct stat* stat)
{
	/* fraction between healthy (1.0) and deadly (0.0) water levels */ 
	float h = ((2 * stat->water) - (3 * stat->grain)) / stat->grain;
	if ( h > 1 ) return 1;
	if ( h < 0 ) return 0;
	return h;
}

float stat_water_cycle(struct stat* stat)
{
	/* amount of water which can be drank or sweat in one minute */
	return 0.00125 * stat->grain *
		stat_hydration(stat) * stat_hydration(stat);
}

float stat_water_loss(float stat_grain)
{
	/* water lost per minute of living */
	return 0.000035 * stat_grain;
}

float stat_health(struct stat* stat)
{
	/* the average of all stats affecting health */
	return stat_hydration(stat);
}

float stat_strength(struct stat* stat)
{
	/* fat burned per minute of work */
	return 0.0004 * stat->grain * stat_health(stat);
}

float stat_fat_loss(float stat_grain)
{
	/* fat burned per minute of living */
	return 0.000008 * stat_grain;
}

/* end of file */
