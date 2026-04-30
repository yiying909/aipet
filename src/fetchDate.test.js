import { jest } from "@jest/globals";
import fetchMock from "jest-fetch-mock";
import { fetchDate, fetchWeather } from "./fetchDate.js";
import assert from 'node:assert';

const SECOND = 1000;
jest.setTimeout(10 * SECOND);

describe("fetchDate", () => {
    beforeEach(() => {
        fetchMock.enableMocks();
    });
    afterEach(() => {
        fetchMock.resetMocks();
        fetchMock.disableMocks();
    });

    it("checking to see what the res.json prints out when fetching from the url", () => {
        const result = await fetchDate();
        assert(result);
        console.log(result);
    });
});

describe("fetchWeather", () => {

});