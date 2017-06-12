import { createReadStream, createWriteStream, readFile } from 'fs'
import { resolve } from 'path'

const csvFilePath: string = resolve ( __dirname, '../../OFSoundAnalyser/bin/data/out.csv')

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

async function main () {
  const csvData: string[] = await readFilePromise(csvFilePath)

  // console.log(csvData)

  // A. which threshold is the best?
  const thresholds: { [key: string]: [number] } = {}
  const csvDataByElements: string[][] = [] // just to make it easier afterwards

  for (const line of csvData) {
    const elements: string[] = line.split(',')
    csvDataByElements.push(elements)
    if (!(`${ elements[2] }` in thresholds)) {
      thresholds[`${elements[2]}`] = [1, 0]
    }
    thresholds[`${elements[2]}`][0] ++
    if (didItMatch(elements)) {
      thresholds[`${elements[2]}`][1] ++
    }

  }

  console.log(thresholds)

  // B. which duration holds the best?
  const durations: {[key: string]: [number]} = {}

  for (const line of csvDataByElements) {
    if (!(`${ line[3] }` in durations)) {
      durations[`${line[3]}`] = [1, 0]
    }
    durations[`${line[3]}`][0] ++
    if (didItMatch(line))  {
      durations[`${line[3]}`][1] ++
    }

  }

  console.log(durations)

  // A., B. combined
  const combined: { [thresholdAndDurations: string]: number[] } = {}

  for (const line of csvDataByElements) {
    const key: string = `t${line[2]}d${line[3]}`
    if (!(key in combined)) {
      combined[key] = [0, 0]
    }
    combined[key][0] ++
    if (didItMatch(line))  {
      combined[key][1] ++
    }

  }

  const ratio: { [key: string]: number } = {}
  Object.keys(combined).map((key: string) => {
    ratio[key] = combined[key][1] / combined[key][0]
  })

  console.log(ratio)
}

main()
