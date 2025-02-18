import { type JSONValue } from "./jsonvalue";

// actually just more detailed representation of Necromancy::Messages::ASDump structure
export class FullDump {
    private score: number = 0;
    private totalTraffic: Array<number> = [];
    private collectedTraffic: Array<number> = [];

    public fromRaw(data: JSONValue) : void {

    }
}