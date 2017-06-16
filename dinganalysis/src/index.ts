import { createReadStream, createWriteStream, readFile, writeFile } from 'fs'
import { resolve } from 'path'

const fileTag = 'res0'
const csvFilePath: string = resolve ( __dirname, `../../OFSoundAnalyser/bin/data/out_${fileTag}.csv`)

function readFilePromise (filePath: string): Promise<string []> {
  return new Promise((resolve, reject) => {
    readFile(filePath, (error, buffer: Buffer) => {
      if (error !== null) {
        return reject(error)
      }
      resolve(buffer.toString().split('\n'))
    })
  })
}

function excludeHidden (dirname: string): boolean {
  return !(dirname.startsWith('.') || dirname.startsWith('random'))
}

function bellFrequencyMap (bellName: string): number {
  switch (bellName) {
    case 'gold' :
      return 2484
    case 'oi' :
      return 2765
    case 'silver' :
      return 2390
    case 'hubway' :
      return 2953
    case 'crane' :
      return 3609
    default :
      return 3000
  }
}

function folderCodeMap (folder: string): string {
  switch (folder) {
    case 'noisy-single' :
      return 'n_1_0'
    case 'noisy-double' :
      return 'n_0_1'
    case 'silent-single' :
      return 's_1_0'
    case 'silent-double' :
      return 's_0_1'
  }
}

function didItMatch (line: string[]): boolean {
  let count = 0
  for (const element of line) {
    count++
    if (element.trim() === 'didPass') {
      return line[count] === '1'
    }
  }
  return false
}

function checkCase (baseObj: any, line: string[], key: string, blacklist: string[] = []): void {

  for (const black of blacklist) {
    if (black === line[0]) {
      return
    }
  }

  if (!(key in baseObj)) {
    baseObj[key] = [0, 0]
  }
  baseObj[key][0] ++

  if (didItMatch(line)) {
    baseObj[key][1] ++
  }

}

async function main () {
  const csvData: string[] = await readFilePromise(csvFilePath)

  // console.log(csvData)

  // X. check for bad recordings
  const recordings: { [key: string]: number[]} = {}
  const csvDataByElements: string[][] = [] // just to make it easier afterwards
  for (const line of csvData) {
    const elements: string[] = line.split(',')
    csvDataByElements.push(elements)
    checkCase (recordings, elements, `${elements[0]}`)
  }

  // console.log(recordings)

  const blacklist: string[] = []
  Object.keys(recordings).map((key: string) => {
    const results = recordings[key]
    const ratio = results[1] / results[0]
    if (results[1] < 1) {
      blacklist.push(key)
    }
  })

  console.log(blacklist)

  // A. which threshold is the best?
  const thresholds: { [key: string]: number[] } = {}

  for (const line of csvDataByElements) {
    checkCase (thresholds, line, `${line[1]}`, blacklist)
  }

  console.log(thresholds)

  // B. which duration holds the best?
  const durations: {[key: string]: [number]} = {}

  for (const line of csvDataByElements) {
    checkCase (durations, line, `${line[2]}`, blacklist)
  }

  console.log(durations)

  // A., B. combined
  const combined: { [thresholdAndDurations: string]: number[] } = {}

  for (const line of csvDataByElements) {
    const key: string = `t${line[1]}d${line[2]}`
    checkCase (combined, line, key, blacklist)
  }

  const ratio: { [key: string]: number } = {}
  Object.keys(combined).map((key: string) => {
    ratio[key] = combined[key][1] / combined[key][0]
  })

  console.log(ratio)

  const result: any = {
    blacklist,
    thresholds,
    durations,
    combined: ratio,
  }

  const resultJSON: string = JSON.stringify(result)

  writeFile(resolve(__dirname, `./out/out_${fileTag}_${Date.now()}.json`), resultJSON, (error) => {
    if (error) {
      return console.error(error)
    }

    console.log('file written')
  })

}

main()
