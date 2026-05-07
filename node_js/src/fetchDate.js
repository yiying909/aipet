// fetch current date and weather

/* Found a free API online that can be used, do need to be careful
of rate-limiting. Gives back date and time information depending on
current time zone which is hardcoded to New York, America. Fetch
returns the date in month/day/year format along with military time
in 24 hour cycle. Returning the time in regular clock format along
with date in month/day/year.
 */
export async function fetchDate() {
    const url = new URL("https://timeapi.io/api/Time/current/zone?timeZone=America/New_York");
    
    const response = await fetch(url.toString());
    if(!response.ok){
        throw new Error(response.statusText);
    }
    const data = await response.json();

    //reconstructing time into regular clock format (not 24 hr military)
    const military_hr = data.hour;
    const min = data.minute;

    const period_of_day = military_hr>=12 ? "PM" : "AM";
    const clock_hr = military_hr % 12 || 12;
    const time = `${clock_hr}:${String(min).padStart(2, "0")} ${period_of_day}`;
    
    return {
        time: time,
        date: data.date
    };
}

/* Found a free API online that can be used, do need to be careful
of rate-limiting. Gives back current_weather information depending on
latitude and longitude, currently hardcoded for Amherst, MA.
Returning the temperature in both celsius and fahrenheit; when fetching
from API originally returns temperature in degrees celsius. 
 */
export async function fetchWeather() {
    const url = new URL("https://api.open-meteo.com/v1/forecast");
    url.searchParams.append("latitude", "42.39");
    url.searchParams.append("longitude", "-72.54");
    url.searchParams.append("current_weather", "true");

    const response = await fetch(url.toString());
    if(!response.ok){
        throw new Error(response.statusText);
    }

    const data = await response.json();
    if(!data.current_weather){
        throw new Error("Can't get the current weather, data is missing.");
    }

    let celsius = data.current_weather.temperature;
    let fahrenheit = Number(((celsius * 9/5) + 32).toFixed(2));
    return {
        celsius: celsius,
        fahrenheit: fahrenheit
    };
}

// testing
// const date = await fetchDate();
// const weather = await fetchWeather();
// console.log(date, weather)