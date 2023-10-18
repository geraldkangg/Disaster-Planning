#include "DisasterPlanning.h"
#include "GUI/SimpleTest.h"
using namespace std;

/* The disaster planning file uses a series of functions along with a map with the city and its neighbors to find
 * the most efficient way to stockpile a map to make sure every city is covered. It recursviely calls itself to check
 * for each possibility until it finds one.
 */

/**
 * @brief getUpdatedLocationsSet - This program returns an updated locations set that takes away the covered locations
 * from the uncovered locations.
 * @param roadNetwork - The map that we are given in the beginning
 * @param neighbor - The neighbor/city we are iterating through to find its neighbors
 * @param uncoveredLocations - The original uncovered locations set that we need to alter
 * @return - A new uncovered locations set that takes away all the covered locations.
 */
Set<string> getUpdatedLocationsSet(const Map<string, Set<string>>& roadNetwork,
                                   const string& neighbor,
                                   const Set<string>& uncoveredLocations) {

    Set<string> updatedUncoveredLocations = uncoveredLocations;
    for (const string& neighborCity : roadNetwork[neighbor] + neighbor) {
        //Iterating through all the neighbor cities of the city/neighbor we need to address
        updatedUncoveredLocations -= neighborCity;
    }
    return updatedUncoveredLocations;
}


/**
 * @brief canBeMadeDisasterReadyRec - This is the recursive call of the wrapper function below.
 * @param roadNetwork - The map we are given with a set of cities and its neighbors.
 * @param numCities - The number of cities we are allowed to stockpile.
 * @param supplyLocations - A set containing the cities we are stockpiling. We need to edit this in our function.
 * @param uncoveredLocations - A set keeping track of the set of uncovered locations we need to address.
 * @return - Whether it is possible to cover the whole map with the amount of cities we have or not.
 */
bool canBeMadeDisasterReadyRec(const Map<string, Set<string>>& roadNetwork,
                               int numCities,
                               Set<string>& supplyLocations,
                               const Set<string>& uncoveredLocations) {

    if (numCities < 0) {
        //This addresses an error. The number of cities cannot be negative.
        error("number of cities cannot be negative.");
    }

    if (uncoveredLocations.isEmpty()) {
        //First Base Case
        return true;
    }

    if (supplyLocations.size() < numCities){
        //Another base case/necessity. We need to make sure we are not infinitely adding supply locations therefore we need this check

        string uncoveredCity = uncoveredLocations.first();
        //Pick a random city

        for (const string& neighbor : roadNetwork[uncoveredCity] + uncoveredCity) {
            //We are iterating through the city we chose AND all of its neighbors

            Set<string> updatedUncoveredLocations = getUpdatedLocationsSet(roadNetwork, neighbor, uncoveredLocations);
            //We find the new updated locations set that would result from this city we are recursively trying

            if (canBeMadeDisasterReadyRec(roadNetwork, numCities, supplyLocations += neighbor, updatedUncoveredLocations)) {
                //Recursive Call
                return true;
                }
            supplyLocations -= neighbor;
            //Backtracking where we tried adding this supply location. If it doesn't work we need to delete it
            }
        }
    return false;
}

/**
 * @brief canBeMadeDisasterReady - Wrapper function that allows us to add another paramter.
 * @param roadNetwork - The map we are given with a set of cities and its neighbors.
 * @param numCities - numCities - The number of cities we are allowed to stockpile.
 * @param supplyLocations - A set containing the cities we are stockpiling. We need to edit this in our function.
 * @return - Whether it is possible to cover the whole map with the amount of cities we have or not.
 */
bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& supplyLocations) {

    Set<string> uncoveredLocations;
    for (const string& city : roadNetwork) {
        uncoveredLocations += city;
    }
    //Creating a set of uncovered locations which initially would be every city in the map.

    return canBeMadeDisasterReadyRec(roadNetwork, numCities, supplyLocations, uncoveredLocations);
}



/* * * * * * * Test Helper Functions Below This Point * * * * * */

/* This is a helper function that's useful for designing test cases. You give it a Map
 * of cities and what they're adjacent to, and it then produces a new Map where if city
 * A links to city B, then city B links back to city A. We recommend using this helper
 * function when writing tests, though you won't need it in your implementation of the main
 * canBeMadeDisasterReady function.
 */
Map<string, Set<string>> makeSymmetric(const Map<string, Set<string>>& source) {
    Map<string, Set<string>> result = source;

    for (const string& from: source) {
        for (const string& to: source[from]) {
            result[from] += to;
            result[to] += from;
        }
    }

    return result;
}

/* This helper function tests whether a city has been covered by a set of supply locations
 * and is used by our testing code. You're welcome to use it in your tests as well!
 */
bool isCovered(const string& city,
               const Map<string, Set<string>>& roadNetwork,
               const Set<string>& supplyLocations) {
    if (supplyLocations.contains(city)) return true;

    for (string neighbor: roadNetwork[city]) {
        if (supplyLocations.contains(neighbor)) return true;
    }

    return false;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */

STUDENT_TEST("Works for four disconnected & group of cities.") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "A-1", "A-2"} },
        { "B", { "B-1" } },
        { "C", { "C-1", "C-2"} },
        { "D", { "D-1"} }
    });

    Set<string> locations0, locations1, locations2, locations3, locations4;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(!canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(!canBeMadeDisasterReady(map, 2, locations2));
    EXPECT(!canBeMadeDisasterReady(map, 3, locations3));
    EXPECT(canBeMadeDisasterReady(map, 4, locations4));
}

STUDENT_TEST("Works when input is duplicated") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "A-1", "A-2"} },
        { "A", { "A-1", "A-2"} },
    });

    Set<string> locations0, locations1, locations2, locations3, locations4;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(canBeMadeDisasterReady(map, 2, locations2));
}


STUDENT_TEST("Works when input is duplicated in a way that is double-linked") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { "A-1"} },
        { "A-1", { "A"} },
    });

    Set<string> locations0, locations1, locations2, locations3, locations4;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(canBeMadeDisasterReady(map, 2, locations2));
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Reports an error if numCities < 0") {
    Set<string> supply;
    EXPECT_ERROR(canBeMadeDisasterReady({}, -137, supply));
}

PROVIDED_TEST("Works for map with no cities.") {
    Set<string> locations;

    /* The number of cities we use really doesn't matter here. */
    EXPECT(canBeMadeDisasterReady({}, 0, locations));
    EXPECT_EQUAL(locations.size(), 0);

    EXPECT(canBeMadeDisasterReady({}, 137, locations));
    EXPECT_EQUAL(locations.size(), 0);
}

PROVIDED_TEST("Works for map with one city.") {
    Map<string, Set<string>> map = makeSymmetric({
         { "Solipsist", {} }
    });

    /* Shouldn't matter how many cities we use, as long as it isn't zero! */
    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT( canBeMadeDisasterReady(map, 1, locations1));
    EXPECT( canBeMadeDisasterReady(map, 2, locations2));
}

PROVIDED_TEST("Works for map with one city, and produces output.") {
    Map<string, Set<string>> map = makeSymmetric({
         { "Solipsist", {} }
    });

    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(canBeMadeDisasterReady(map, 2, locations2));

    /* Don't check locations0; since the function returned false, the values there
     * can be anything.
     */
    Set<string> expected = { "Solipsist" };
    EXPECT_EQUAL(locations1, expected);
    EXPECT_EQUAL(locations2, expected);
}

PROVIDED_TEST("Works for map with two linked cities.") {
    Map<string, Set<string>> map = makeSymmetric({
         { "A", { "B" } },
         { "B", {     } }
    });

    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(canBeMadeDisasterReady(map, 2, locations2));
}

PROVIDED_TEST("Works for map with two linked cities, and produces output.") {
    Map<string, Set<string>> map = makeSymmetric({
         { "A", { "B" } },
    });

    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(canBeMadeDisasterReady(map, 2, locations2));

    EXPECT_EQUAL(locations1.size(), 1);
    EXPECT(locations1.isSubsetOf({"A", "B"}));

    EXPECT(locations2.size() <= 2);
    EXPECT(locations2.isSubsetOf({"A", "B"}));
}

PROVIDED_TEST("Works for four disconnected cities.") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { } },
        { "B", { } },
        { "C", { } },
        { "D", { } }
    });

    Set<string> locations0, locations1, locations2, locations3, locations4;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(!canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(!canBeMadeDisasterReady(map, 2, locations2));
    EXPECT(!canBeMadeDisasterReady(map, 3, locations3));
    EXPECT(canBeMadeDisasterReady(map, 4, locations4));
}

PROVIDED_TEST("Works for four disconnected cities, and produces output.") {
    Map<string, Set<string>> map = makeSymmetric({
        { "A", { } },
        { "B", { } },
        { "C", { } },
        { "D", { } }
    });

    Set<string> locations0, locations1, locations2, locations3, locations4;
    EXPECT(!canBeMadeDisasterReady(map, 0, locations0));
    EXPECT(!canBeMadeDisasterReady(map, 1, locations1));
    EXPECT(!canBeMadeDisasterReady(map, 2, locations2));
    EXPECT(!canBeMadeDisasterReady(map, 3, locations3));
    EXPECT(canBeMadeDisasterReady(map, 4, locations4));

    Set<string> expected = { "A", "B", "C", "D" };
    EXPECT_EQUAL(locations4, expected);
}

PROVIDED_TEST("Can solve ethene example, regardless of ordering.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over the cities when making decisions is sensitive
     * to the order of those cities' names. This test looks at a map like
     * this one, trying out all possible orderings of the city names:
     *
     *             *
     *             |
     *        * -- * -- * -- *
     *                  |
     *                  *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * cities. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly
     * covering and uncovering cities at each step. On the other hand, if
     * your code runs into issues here, it may indicate that the way in which
     * you're covering and uncovering cities accidentally uncovers a city that
     * you have previously covered.
     */
    Vector<string> cities = { "A", "B", "C", "D", "E", "F" };
    do {
        /* Ethene arrangement:
         *
         *        0
         *       1234
         *         5
         */
        Map<string, Set<string>> map = makeSymmetric({
            { cities[2], { cities[0], cities[1], cities[3] } },
            { cities[3], { cities[4], cities[5] } }
        });

        /* We should be able to cover everything with two cities:
         * city 2 and city 3.
         */
        Set<string> chosen;
        EXPECT(canBeMadeDisasterReady(map, 2, chosen));

        /* We should not be able to cover everything with one city. */
        chosen.clear();
        EXPECT(!canBeMadeDisasterReady(map, 1, chosen));
    } while (next_permutation(cities.begin(), cities.end()));
}

PROVIDED_TEST("Can solve ethene example, regardless of ordering, and produces output.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over the cities when making decisions is sensitive
     * to the order of those cities' names. This test looks at a map like
     * this one, trying out all possible orderings of the city names:
     *
     *             *
     *             |
     *        * -- * -- * -- *
     *                  |
     *                  *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * cities. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly
     * covering and uncovering cities at each step. On the other hand, if
     * your code runs into issues here, it may indicate that the way in which
     * you're covering and uncovering cities accidentally uncovers a city that
     * you have previously covered.
     */
    Vector<string> cities = { "A", "B", "C", "D", "E", "F" };
    do {
        /* Ethene arrangement:
         *
         *        0
         *       1234
         *         5
         */
        Map<string, Set<string>> map = makeSymmetric({
            { cities[2], { cities[0], cities[1], cities[3] } },
            { cities[3], { cities[4], cities[5] } }
        });

        /* We should be able to cover everything with two cities:
         * city 2 and city 3.
         */
        Set<string> chosen;
        EXPECT(canBeMadeDisasterReady(map, 2, chosen));

        EXPECT_EQUAL(chosen.size(), 2);
        EXPECT(chosen.contains(cities[2]));
        EXPECT(chosen.contains(cities[3]));

        /* We should not be able to cover everything with one city. */
        chosen.clear();
        EXPECT(!canBeMadeDisasterReady(map, 1, chosen));
    } while (next_permutation(cities.begin(), cities.end()));
}

PROVIDED_TEST("Works for six cities in a line, regardless of order.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over the cities when making decisions is sensitive
     * to the order of those cities' names. This test looks at a map like
     * this one, trying out all possible orderings of the city names:
     *
     *        * -- * -- * -- * -- * -- *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * cities in a line. If your code is able to solve the problem correctly
     * for all of those orderings, there's a good chance that you're correctly
     * covering and uncovering cities at each step. On the other hand, if
     * your code runs into issues here, it may indicate that the way in which
     * you're covering and uncovering cities accidentally uncovers a city that
     * you have previously covered.
     */
    Vector<string> cities = { "A", "B", "C", "D", "E", "F" };
    do {
        /* Linear arrangement. */
        Map<string, Set<string>> map;
        for (int i = 1; i + 1 < cities.size(); i++) {
            map[cities[i]] = { cities[i - 1], cities[i + 1] };
        }

        map = makeSymmetric(map);

        /* We should be able to cover everything with two cities, specifically,
         * the cities one spot in from the two sides.
         */
        Set<string> chosen;
        EXPECT(canBeMadeDisasterReady(map, 2, chosen));

        /* We should not be able to cover everything with one city. */
        chosen.clear();
        EXPECT(!canBeMadeDisasterReady(map, 1, chosen));
    } while (next_permutation(cities.begin(), cities.end()));
}

PROVIDED_TEST("Works for six cities in a line, regardless of order, and produces output.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over the cities when making decisions is sensitive
     * to the order of those cities' names. This test looks at a map like
     * this one, trying out all possible orderings of the city names:
     *
     *        * -- * -- * -- * -- * -- *
     *
     * There are 6! = 720 possible permutations of the ordering of these six
     * cities in a line. If your code is able to solve the problem correctly
     * for all of those orderings, there's a good chance that you're correctly
     * covering and uncovering cities at each step. On the other hand, if
     * your code runs into issues here, it may indicate that the way in which
     * you're covering and uncovering cities accidentally uncovers a city that
     * you have previously covered.
     */
    Vector<string> cities = { "A", "B", "C", "D", "E", "F" };
    do {
        /* Linear arrangement. */
        Map<string, Set<string>> map;
        for (int i = 1; i + 1 < cities.size(); i++) {
            map[cities[i]] = { cities[i - 1], cities[i + 1] };
        }

        map = makeSymmetric(map);

        /* We should be able to cover everything with two cities, specifically,
         * the cities one spot in from the two sides.
         */
        Set<string> chosen;
        EXPECT(canBeMadeDisasterReady(map, 2, chosen));

        EXPECT_EQUAL(chosen.size(), 2);
        EXPECT(chosen.contains(cities[1]));
        EXPECT(chosen.contains(cities[4]));

        /* We should not be able to cover everything with one city. */
        chosen.clear();
        EXPECT(!canBeMadeDisasterReady(map, 1, chosen));
    } while (next_permutation(cities.begin(), cities.end()));
}

/* The "Don't Be Greedy" sample world. */
const Map<string, Set<string>> kDontBeGreedy = makeSymmetric({
    { "A", { "B" } },
    { "B", { "C", "D" } },
    { "C", { "D" } },
    { "D", { "F", "G" } },
    { "E", { "F" } },
    { "F", { "G" } },
});

PROVIDED_TEST("Solves \"Don't be Greedy\" from the handout.") {
    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(kDontBeGreedy, 0, locations0));
    EXPECT(!canBeMadeDisasterReady(kDontBeGreedy, 1, locations1));
    EXPECT( canBeMadeDisasterReady(kDontBeGreedy, 2, locations2));
}

PROVIDED_TEST("Solves \"Don't be Greedy\" from the handout, and produces output.") {
    Set<string> locations0, locations1, locations2;
    EXPECT(!canBeMadeDisasterReady(kDontBeGreedy, 0, locations0));
    EXPECT(!canBeMadeDisasterReady(kDontBeGreedy, 1, locations1));
    EXPECT( canBeMadeDisasterReady(kDontBeGreedy, 2, locations2));

    Set<string> expected = {"B", "F"};
    EXPECT_EQUAL(locations2, expected);
}

PROVIDED_TEST("Solves \"Don't be Greedy,\" regardless of ordering, and produces output.") {
    /* Because Map and Set internally store items in sorted order, the order
     * in which you iterate over the cities when making decisions is sensitive
     * to the order of those cities' names. This test looks at a map like
     * this one, trying out all possible orderings of the city names:
     *
     *     0       4
     *     |       |
     *     1 - 2 - 3
     *      \ / \ /
     *       5   6
     *
     * There are 7! = 5,040 possible permutations of the ordering of these seven
     * cities. If your code is able to solve the problem correctly for all of
     * those orderings, there's a good chance that you're correctly
     * covering and uncovering cities at each step. On the other hand, if
     * your code runs into issues here, it may indicate that the way in which
     * you're covering and uncovering cities accidentally uncovers a city that
     * you have previously covered.
     */
    Vector<string> cities = { "A", "B", "C", "D", "E", "F", "G" };
    do {
        Map<string, Set<string>> map = makeSymmetric({
            { cities[1], { cities[0], cities[2], cities[5] } },
            { cities[2], { cities[3], cities[5], cities[6] } },
            { cities[3], { cities[4], cities[6] } },
        });

        /* We should be able to cover everything with two cities. */
        Set<string> chosen;
        EXPECT(canBeMadeDisasterReady(map, 2, chosen));

        /* Those cities should be 1 and 3. */
        EXPECT_EQUAL(chosen, (Set<string>{ cities[1], cities[3] }));

        /* We should not be able to cover everything with one city. */
        chosen.clear();
        EXPECT(!canBeMadeDisasterReady(map, 1, chosen));
    } while (next_permutation(cities.begin(), cities.end()));
}

PROVIDED_TEST("Stress test: 6 x 6 grid. (This should take at most a few seconds.)") {
    Map<string, Set<string>> grid;

    /* Build the grid. */
    char maxRow = 'F';
    int  maxCol = 6;
    for (char row = 'A'; row <= maxRow; row++) {
        for (int col = 1; col <= maxCol; col++) {
            if (row != maxRow) {
                grid[row + to_string(col)] += (char(row + 1) + to_string(col));
            }
            if (col != maxCol) {
                grid[row + to_string(col)] += (char(row) + to_string(col + 1));
            }
        }
    }
    grid = makeSymmetric(grid);

    Set<string> locations;
    EXPECT(canBeMadeDisasterReady(grid, 10, locations));
}

PROVIDED_TEST("Stress test output. (This should take at most a few seconds.)") {
    Map<string, Set<string>> grid;

    /* Build the grid. */
    char maxRow = 'F';
    int  maxCol = 6;
    for (char row = 'A'; row <= maxRow; row++) {
        for (int col = 1; col <= maxCol; col++) {
            if (row != maxRow) {
                grid[row + to_string(col)] += (char(row + 1) + to_string(col));
            }
            if (col != maxCol) {
                grid[row + to_string(col)] += (char(row) + to_string(col + 1));
            }
        }
    }
    grid = makeSymmetric(grid);

    Set<string> locations;
    EXPECT(canBeMadeDisasterReady(grid, 10, locations));

    for (char row = 'A'; row <= maxRow; row++) {
        for (int col = 1; col <= maxCol; col++) {
            EXPECT(isCovered(row + to_string(col), grid, locations));
        }
    }
}
